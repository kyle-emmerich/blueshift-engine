#pragma once
#include "Core/IApplication.h"
#include "Core/Utility/ConfigFile.h"
#include "Graphics/RenderSystem.h"

namespace Blueshift {
	namespace Core {

		
		struct EngineParameters {
			ApplicationType Type;
			std::string ApplicationName;

			IApplication* Application;

			Utility::ConfigFile EngineConfig;

			struct {
				bool Rendering;
				bool Input;
				bool Audio;
				bool Physics;
				bool Networking;
				bool AI;
				bool API;
				bool Storage;
				bool Database;
			} Subsystems;
		};

		class Engine {
		private:
			Graphics::RenderSystem* render = nullptr;

			EngineParameters* parameters;
		
			bool stop = false;

			static Engine* _instance;
		public:
			inline static Engine& Get() {
				return *_instance;
			}
			inline static void Destroy() {
				delete _instance;
			}
			Engine(EngineParameters* Parameters);
			~Engine();

			Graphics::RenderSystem& GetRenderSystem() const {
				return *render;
			}

			const EngineParameters* GetParameters() const {
				return parameters;
			}
			
		};

	}
}