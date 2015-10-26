#pragma once
#include "Platform/PlatformSpecific.h"
#include "Scene/CameraComponent.h"
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

			Scene::CameraComponent* camera;
		public:
			RenderWindow(uint32_t Width = 1280, uint32_t Height = 720);
			virtual ~RenderWindow();

			void PreRender();
			void PostRender();

			inline double GetAspectRatio() {
				return (double)this->view_width / (double)this->view_height;
			}

			inline void SetCamera(Scene::CameraComponent* camera) { this->camera = camera; }
			inline Scene::CameraComponent* GetCamera() const { return camera; }

			inline uint8_t GetViewID() const {
				return view_id;
			}
		};

	}
}