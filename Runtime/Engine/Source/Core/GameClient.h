#pragma once
#include "Core/BaseApplication.h"

namespace Blueshift {
	namespace Core {

		class GameClient : public BaseApplication {
		private:
			Graphics::RenderWindow* main_window;
		public:
			inline GameClient() : BaseApplication() {} 

			virtual EngineParameters* GetEngineSetup();

			virtual void Initialize();
			virtual void Shutdown();

			virtual bool Update(double);
			
		};

	}
}