#pragma once
#include "Core/BaseApplication.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/RenderWindow.h"
#include <condition_variable>

namespace Blueshift {
	namespace Core {

		class GameClient : public BaseApplication {
		protected:
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