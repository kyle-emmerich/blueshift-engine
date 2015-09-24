#pragma once
#include <map>
#include <vector>
#include "Input/IInputDevice.h"

namespace Blueshift {
	namespace Input {

		class InputManager {
		protected:
			static InputManager* _instance;

			std::map<InputDeviceType, std::vector<IInputDevice*>> open_devices;
		public:
			inline static InputManager& Get() {
				return *_instance;
			}
			 InputManager();
			~InputManager();


		};

	}
}