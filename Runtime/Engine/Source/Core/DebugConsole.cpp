#include "Core/DebugConsole.h"
#include "Core/Engine.h"
#include "Input/Windows/Keyboard_WIN.h"
#include "bgfx/bgfx.h"

using namespace Blueshift;
using namespace Core;

DebugConsole::DebugConsole(size_t num_shown, size_t max_size)
	: start(0), show(num_shown), max_size(max_size), visible(false) {
	memset(input, ' ', 256);
	Input::Devices::Keyboard::TextInput.Subscribe([this](const char& c) {
		this->text_input(c);
	});
}

DebugConsole::~DebugConsole() {

}

void DebugConsole::Render() {
	bgfx::dbgTextClear();
	if (!visible) {
		return;
	}
	bgfx::dbgTextPrintf(1, 1, 0x0f, "Blueshift Engine v%d.%d.%d %s",
		Blueshift::EngineVersion[0],
		Blueshift::EngineVersion[1],
		Blueshift::EngineVersion[2],
		Blueshift::EngineDebugBuild ? "DEBUG BUILD" : "");

	uint16_t y = 3;

	if (start > 0) {
		bgfx::dbgTextPrintf(1, y, 0x0f, "^");
	}

	for (size_t i = start; i < start + show; i++) {
		if (i >= lines.size()) {
			break;
		} else {
			bgfx::dbgTextPrintf(3, y, lines[i].attr, lines[i].text.c_str());
		}
		y++;
	}
	y = 3 + show + 2;
	bgfx::dbgTextPrintf(2, y, 0x0f, "> %s", input);
	bgfx::dbgTextPrintf(2 + 2 + input_cursor, y, 0xf0, " ");
}

void DebugConsole::write_line(const std::string& line, uint8_t attr) {
	if (start == lines.size() - show) {
		start++;
	} else if (lines.size() == max_size) {
		lines.pop_front();
	}
	lines.push_back({
		line, attr
	});
}

void DebugConsole::text_input(char c) {
	if (!visible)
		return;
	if (c == 8) {
		//backspace
		if (input_cursor > 0) {
			input[--input_cursor] = ' ';
		}
		return;
	} else if (c == 13) {
		//enter
		//submit this to the lua interpreter
		if (lua != nullptr) {
			char* input_buf = new char[256];
			memset(input_buf, 0, 256);
			memcpy(input_buf, input, input_cursor);
			Write(Formatter() << "> " << input_buf, 0x08);
			try {
				lua->ExecuteString(input_buf, "console");
			} catch (...) {}
			delete[] input_buf;
		}
		memset(input, ' ', 256);
		input_cursor = 0;
		return;
	} else if (c == '`') {
		return;
	}
	input[input_cursor++] = c;
}

void DebugConsole::Write(const std::string& str, uint8_t attr) {
	char buf[80]; memset(buf, 0, 80);
	size_t pos = 0;
	for (size_t i = 0; i < str.length(); i++) {
		if (pos >= 80) {
			//flush the buf
			write_line(buf, attr);
			memset(buf, 0, 80);
			pos = 0;
			
		}
		if (str[i] == '\t') {
			//let's skip over four spaces until we hit the end of a line
			size_t before = (pos / 4) * 4;
			size_t after = before + 4;
			size_t num_left = after - pos;

			//if we're only one past, just treat it as a space
			if (pos == before) {
				num_left = 1;
			}
			for (size_t j = 0; j < num_left && pos < 80; j++) {
				buf[pos] = ' ';
				pos++;
			}
		} else if (str[i] == '\n') {
			//flush the buf
			write_line(buf, attr);
			memset(buf, 0, 80);
			pos = 0;
		} else if (str[i] == '\r') {
			//don't waste a space in the buffer on this
			continue;
		} else {
			//otherwise, just write it.
			buf[pos++] = str[i];
		}
	}
	if (pos > 0) {
		//flush the toilet
		write_line(buf, attr);
	}
}