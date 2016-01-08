#pragma once
#include "Core/BaseApplication.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/RenderWindow.h"
#include <condition_variable>

namespace Blueshift {
	namespace Core {

		/*! \class GameClient
			\brief Encapsulates behavior typical in a game application.

			The GameClient is the basis of any real-time game with graphics. It initializes a RenderSystem and a
			RenderWindow for you according to the configuration file "app.cfg" found in any readable directory.

			\sa BaseApplication, RenderSystem, RenderWindow
		*/
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