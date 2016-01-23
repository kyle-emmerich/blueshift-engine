#pragma once
#include "Application/IApplication.h"
#include "Core/Engine.h"
#include "Graphics/Graphics.h"
#include "Core/Timing/Stopwatch.h"
#include <atomic>

namespace Blueshift {
	namespace Application {

		/*! \class BaseApplication
			\brief Encapsulates application behavior, such as running a main loop.

			The BaseApplication is a barebones method of running the game engine. It will create an instance
			of the engine and that's about it.

			\sa GameServer, GameClient
		*/
		class BaseApplication : public IApplication {
		private:
			std::atomic<bool> running = true;
			
		protected:
			Scene::SceneGraph* graph = nullptr;
		public:
			BaseApplication();
			virtual ~BaseApplication();

			void Start(int argc, char* argv[]);

			/*!
				\brief Allows applications to configure the engine before it starts.

				Allows the application to setup parameters for the engine before it is instantiated.
				If extending, please call BaseApplication::GetEngineSetup() to get a pointer to an
				allocated EngineParameters object with defaults.
				\return A pointer to an EngineParameters object to be filled out with desired parameters.
						Pointer is owned by the engine, do not delete.
			*/
			virtual Core::EngineParameters* GetEngineSetup();

			inline virtual void RegisterFileTypes(Storage::AssetSystem* Assets);
			inline virtual void Initialize() {}
			inline virtual void InitializeRenderData() {}
			inline virtual void DestroyRenderData() {}
			inline virtual void Shutdown() {}
			
			inline virtual bool Update(double) { return true; }

			inline virtual bool IsRunning() {
				return running;
			}
			Core::Engine* Engine;
			Utility::ConfigFile* AppConfig;
			Utility::ConfigFile* UserConfig;
		};
	}
}