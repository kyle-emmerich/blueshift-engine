#pragma once
#include "Input/IInputDevice.h"

namespace Blueshift {
	namespace Input {
		namespace Devices {

			class Keyboard : public IInputDevice, IInputDeviceCapabilityButtons {
			protected:
				bool matrix[(size_t)ButtonName::Max];
			public:
				//Implement buttons capability
				bool IsButtonDown(ButtonName Button);

				//Keyboard-exclusive functionality
				Core::Event<void(char)> TextInput;

				//Implement input device
				static Keyboard* Open();
				static void Close(Keyboard*);
			};

		}
	}
}