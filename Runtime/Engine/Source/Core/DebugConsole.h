#pragma once
#include "Scripting/LuaState.h"
#include <string>
#include <deque>

namespace Blueshift {
	namespace Core {

		/*! \class DebugConsole
			\brief Allows viewing of debug output and execution of Lua at runtime.

			\sa LuaState
		*/
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

			/*! \brief Sets the LuaState that will execute input. */
			inline void SetLuaState(Scripting::LuaState* state) {
				lua = state;
			}
			/*! \brief Returns the LuaState responsible for executing input. */
			inline Scripting::LuaState* GetLuaState() {
				return lua;
			}

			/*! \brief Renders the debug console, only call from render thread. */
			void Render();

			/*! \brief Writes a string to the debug console.
			
				\param str The string to be written, use \n for new-line.
				\param attr foreground and background color using standard VGA colors.
			*/
			void Write(const std::string& str, uint8_t attr = 0x0f);

			/*! \brief Queries whether or not the debug console is currently visible.
			*/
			inline bool IsVisible() const { return visible; }
			/*! \brief Sets the visibility of the debug console. 
			*/
			inline void SetVisible(bool visible) { this->visible = visible; }
		};

	}
}