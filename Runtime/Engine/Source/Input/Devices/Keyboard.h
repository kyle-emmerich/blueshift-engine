#pragma once
#include "Input/IInputDevice.h"

namespace Blueshift {
	namespace Input {
		namespace Devices {

			class Keyboard : public IInputDevice, IInputDeviceCapabilityButtons {
			protected:
				bool matrix[(size_t)ButtonName::Max];

				
			public:
				Keyboard();
				~Keyboard();

				//Implement buttons capability
				bool IsButtonDown(ButtonName Button);

				//Keyboard-exclusive functionality
				Core::Event<void(char)> TextInput;

				
			};
		}

		template<typename T>
		Devices::Keyboard* OpenDevice<Keyboard>() {
			return new Devices::Keyboard;
		}
		
		template<typename T>
		void CloseDevice<Keyboard>(Keyboard* Device) {
			delete Device;
		}
	}
}