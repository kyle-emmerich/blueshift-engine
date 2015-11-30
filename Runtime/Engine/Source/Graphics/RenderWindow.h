#pragma once
#include "Platform/PlatformSpecific.h"
#include "Graphics/CameraComponent.h"
#include <thread>

namespace Blueshift {
	namespace Graphics {

		class RenderWindow : public Platform::Window {
		protected:
			bool is_primary = false;

			bool dirty_viewport = true;
			uint8_t view_id;
			uint32_t view_width;
			uint32_t view_height;
			virtual void resize_viewport(uint32_t width, uint32_t height);

			Scene::SceneGraph* graph;
			Scene::Component::Handle camera;
		public:
			RenderWindow(uint32_t Width = 1280, uint32_t Height = 720, Scene::SceneGraph* scene = nullptr);
			virtual ~RenderWindow();

			void PreRender();
			void PostRender();

			inline float GetAspectRatio() {
				return (float)this->view_width / (float)this->view_height;
			}

			inline void SetCamera(const Scene::Component::Handle& camera) { this->camera = camera; }
			inline const Scene::Component::Handle& GetCamera() const { return camera; }

			inline void SetSceneGraph(Scene::SceneGraph* graph) { this->graph = graph; }
			inline Scene::SceneGraph* GetSceneGraph() const { return graph; }

			inline uint8_t GetViewID() const {
				return view_id;
			}
		};

	}
}