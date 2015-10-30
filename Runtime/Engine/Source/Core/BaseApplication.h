#pragma once
#include "Core/IApplication.h"
#include "Core/Engine.h"
#include "Core/Timing/Stopwatch.h"

namespace Blueshift {
	namespace Core {

		class BaseApplication : public IApplication {
		private:
			bool running = true;
		protected:
			
		public:
			BaseApplication();
			virtual ~BaseApplication();

			void Start(int argc, char* argv[]);

			virtual EngineParameters* GetEngineSetup();

			inline virtual void Initialize() {}
			inline virtual void Shutdown() {}
			
			inline virtual bool Update(double) { return true; }

			inline virtual bool IsRunning() {
				return running;
			}
			Engine* Engine;
			Timing::Stopwatch Timer;
			Utility::ConfigFile* AppConfig;
			Utility::ConfigFile* UserConfig;
			int DesiredFramerate = 60;
		};
	}
}