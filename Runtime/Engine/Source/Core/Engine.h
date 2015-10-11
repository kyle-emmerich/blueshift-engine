#pragma once
#include "Core/Utility/ConfigFile.h"
#include "Graphics/RenderSystem.h"


namespace Blueshift {
	namespace Core {

		class Engine {
		private:
			Graphics::RenderSystem* render = nullptr;

			bool stop = false;

			static Engine* _instance;
		public:
			inline static Engine& Get() {
				return *_instance;
			}
			Engine(bool WithRendering = true);
			~Engine();

			bool HasRenderSystem() const {
				return render != nullptr;
			}
			Graphics::RenderSystem& GetRenderSystem() const {
				return *render;
			}

	

			int Run();
			inline void Stop() { stop = true; }
			inline bool IsStopped() { return stop; }

			Utility::ConfigFile EngineConfig;
			Utility::ConfigFile ApplicationConfig;
		};

	}
}