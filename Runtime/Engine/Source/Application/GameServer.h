#pragma once
#include "Application/BaseApplication.h"

namespace Blueshift {
	namespace Application {

		class GameServer : public BaseApplication {
		public:
			virtual EngineParameters* GetEngineSetup();

			virtual void Initialize();
			virtual void Shutdown();

			virtual bool Update();
		};

	}
}