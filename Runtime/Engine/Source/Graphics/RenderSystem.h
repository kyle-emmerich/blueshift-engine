#pragma once
#include "Core/Engine.h"
#include "Platform/DisplayInfo.h"
#include "Graphics/RenderWindow.h"

#include <thread>
#include <memory>
#include <vector>

namespace Blueshift {
	namespace Graphics {

		/*!	\class RenderSystem
			\brief Subsystem that handles all rendering in the engine and keeps track of active RenderWindow instances.

			This system also implements a rendering thread on which all render commands are sent.
		*/
		class RenderSystem : public Core::ISubsystem {
		private:
			std::vector<Platform::DisplayInfo> displays;
			size_t primary_display_index;

			std::vector<Graphics::RenderWindow*> render_windows;
			uint8_t current_view_id;

			std::thread render_thread;
			void render_thread_func();
		public:
			RenderSystem(Core::Engine* engine);
			virtual ~RenderSystem();

			template<typename T>
			inline void ProcessComponents(Scene::SceneGraph* graph) {
				auto it = graph->Begin<T>();
				for (it; it != graph->End<T>(); ++it) {
					auto x = *it;
					it->Render(this);
				}
			}

			/*!	\brief Adds information about an available display. Should only be called by engine code.
			*/
			void AddAvailableDisplayInfo(Platform::DisplayInfo&& Display);

			/*! \brief Retrieves the current primary display. Created windows will move to this display.
			*/
			const Platform::DisplayInfo& GetPrimaryDisplay() const;
			/*! \brief Retrieves a display by the order in which it was added to the list; usually assigned by the OS.
			*/
			const Platform::DisplayInfo& GetDisplay(size_t index) const;

			const std::vector<Platform::DisplayInfo>& GetDisplays() const;
			inline const size_t GetPrimaryDisplayID() const { return primary_display_index; }

			void SetPrimaryDisplay(const Platform::DisplayInfo& info);
			void SetPrimaryDisplay(size_t index);

			void AddRenderWindow(Graphics::RenderWindow* Window);
			Graphics::RenderWindow& GetPrimaryRenderWindow() const;
			const std::vector<Graphics::RenderWindow*>& GetRenderWindows() const;

			void WaitRenderThread();

			inline uint8_t GetCurrentViewID() const {
				return this->current_view_id;
			}
		};

	}
}