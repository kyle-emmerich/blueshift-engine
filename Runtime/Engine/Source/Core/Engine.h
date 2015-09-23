#pragma once
#include "Platform/DisplayInfo.h"
#include "Core/BasicTypes/Array.h"
#include "Core/Utility/ConfigFile.h"
#include "Graphics/RenderWindow.h"
#include <thread>
#include <memory>
#include <vector>

namespace Blueshift {
	namespace Core {

		class Engine {
		private:
			std::vector<Platform::DisplayInfo> displays;
			size_t primary_display_index;

			std::vector<Graphics::RenderWindow*> render_windows;

			static Engine* _instance;

			std::thread render_thread;
			void render_thread_func();
			bool stop = false;
		public:
			inline static Engine& Get() {
				return *_instance;
			}
			Engine();
			~Engine();

			void AddAvailableDisplayInfo(Platform::DisplayInfo&& Display);

			const Platform::DisplayInfo& GetPrimaryDisplay() const;
			const Platform::DisplayInfo& GetDisplay(size_t index) const;
			const std::vector<Platform::DisplayInfo>& GetDisplays() const;

			void SetPrimaryDisplay(const Platform::DisplayInfo& info);
			void SetPrimaryDisplay(size_t index);

			void AddRenderWindow(Graphics::RenderWindow* Window);
			Graphics::RenderWindow& GetPrimaryRenderWindow() const;
			const std::vector<Graphics::RenderWindow*>& GetRenderWindows() const;

			int Run();
			inline void Stop() { stop = true; }

			Utility::ConfigFile ApplicationConfig;
		};

	}
}