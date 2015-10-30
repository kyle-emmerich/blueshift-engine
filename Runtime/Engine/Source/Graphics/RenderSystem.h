#pragma once
#include "Core/ISubsystem.h"
#include "Platform/DisplayInfo.h"
#include "Core/BasicTypes/Array.h"
#include "Core/Utility/ConfigFile.h"
#include "Graphics/RenderWindow.h"
#include "Graphics/Model/StaticMeshData.h"
#include "bgfx/bgfx.h"
#include <thread>
#include <memory>
#include <vector>

namespace Blueshift {
	namespace Graphics {

		class RenderSystem : public Core::ISubsystem {
		private:
			std::vector<Platform::DisplayInfo> displays;
			size_t primary_display_index;

			std::vector<Graphics::RenderWindow*> render_windows;
			uint8_t current_view_id;

			std::thread render_thread;
			void render_thread_func();
		public:
			RenderSystem();
			~RenderSystem();

			RenderSystem(RenderSystem&) = delete;
			RenderSystem(RenderSystem&&) = delete;

			void AddAvailableDisplayInfo(Platform::DisplayInfo&& Display);

			const Platform::DisplayInfo& GetPrimaryDisplay() const;
			const Platform::DisplayInfo& GetDisplay(size_t index) const;
			const std::vector<Platform::DisplayInfo>& GetDisplays() const;

			void SetPrimaryDisplay(const Platform::DisplayInfo& info);
			void SetPrimaryDisplay(size_t index);

			void AddRenderWindow(Graphics::RenderWindow* Window);
			Graphics::RenderWindow& GetPrimaryRenderWindow() const;
			const std::vector<Graphics::RenderWindow*>& GetRenderWindows() const;

			void WaitRenderThread();

			inline uint8_t GetCurrentViewID() const {
				return current_view_id;
			}
		};

	}
}