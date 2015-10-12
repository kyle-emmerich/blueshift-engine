#pragma once
#include "Core/BaseApplication.h"

namespace Blueshift {
	namespace Core {

		class GameServer : public BaseApplication {
		public:
			virtual EngineParameters* GetEngineSetup();

			virtual void Initialize();
			virtual void Shutdown();

			virtual bool Update();
		};

	}
}