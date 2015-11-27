#pragma once
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
		public:
			DebugConsole(size_t num_shown = 10, size_t max_size = 100);
			~DebugConsole();

			void Render();

			void Write(const std::string& str, uint8_t attr = 0x0f);

			inline bool IsVisible() const { return visible; }
			inline void SetVisible(bool visible) { this->visible = visible; }
		};

	}
}