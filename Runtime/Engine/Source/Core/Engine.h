#pragma once
#include <mutex>

namespace Blueshift {
	namespace Core {
		/*!	\class Engine
			\brief The Engine class encapsulates the state of the game engine and provides access to shared subsystems.
		
			Since the engine is the very core of the process, it is a singleton accessible anywhere. All resources
			contained by this class will be protected by a mutual exclusion lock that must be managed by any code
			accessing it.
		*/
		class Engine {
		private:

		public:
			 Engine();
			~Engine();


		};
	}
}
