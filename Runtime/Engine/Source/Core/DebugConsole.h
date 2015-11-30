#pragma once
#include "Scripting/LuaState.h"
#include <string>
#include <deque>

namespace Blueshift {
	namespace Core {

		class DebugConsole {
		private:
			struct line {
				std::string text;
				uint8_t attr;
			};

			std::deque<line> lines;
			size_t start;
			size_t show;
			size_t max_size;
			bool visible = false;

			char input[256];
			size_t input_cursor = 0;
			Scripting::LuaState* lua = nullptr;

			void write_line(const std::string& line, uint8_t attr);
			void text_input(char c);
		public:
			DebugConsole(size_t num_shown = 10, size_t max_size = 100);
			~DebugConsole();

			inline void SetLuaState(Scripting::LuaState* state) {
				lua = state;
			}
			inline Scripting::LuaState* GetLuaState() {
				return lua;
			}

			void Render();

			void Write(const std::string& str, uint8_t attr = 0x0f);

			inline bool IsVisible() const { return visible; }
			inline void SetVisible(bool visible) { this->visible = visible; }
		};

	}
}