#include "Core/DebugConsole.h"
#include "Core/Engine.h"
#include "bgfx/bgfx.h"

using namespace Blueshift;
using namespace Core;

DebugConsole::DebugConsole(size_t num_shown, size_t max_size)
	: start(0), show(num_shown), max_size(max_size), visible(false) {

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
}

void DebugConsole::Write(const std::string& str, uint8_t attr) {
	if (start == lines.size() - show) {
		start++;
	} else if (lines.size() == max_size) {
		lines.pop_front();
	}
	lines.push_back({
		str, attr
	});
}