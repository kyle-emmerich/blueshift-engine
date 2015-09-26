#pragma once
#ifdef _WIN32 //Windows platform
#include "Input/IInputDevice.h"
#include "Platform/IWindow.h"
#include "Platform/Windows/Window_WIN.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <map>

namespace Blueshift {
	namespace Input {
		namespace Devices {

			class Keyboard : public IInputDevice, DeviceCapability::Buttons {
			protected:
				static std::vector<Keyboard*> all_keyboard_devices;
				static std::map<HANDLE, size_t> device_map;

				static Keyboard* primary;
				static bool primary_found;

				bool matrix[(size_t)ButtonName::Max];

				HANDLE device_handle;
			public:
				Keyboard();
				~Keyboard();

				static Core::Event<char> TextInput;

				static Keyboard* Primary() { return primary; }

				//Implement buttons capability
				bool IsButtonDown(ButtonName Button);
				
				//Implement device
				virtual std::string GetName() { return "Keyboard"; }
				virtual void Poll() {}

				//Windows implementation
				static Keyboard* _get_from_handle(HANDLE handle) {
					return device_map.find(handle) != device_map.end() ? all_keyboard_devices[device_map[handle]] : nullptr;
				}
				void _pass_event(RAWKEYBOARD* data);

				static void Register(Platform::IWindow* Window) {
					//On Windows, this means we register USB devices that
					//are on generic desktop controls and are considered keyboards.
					RAWINPUTDEVICE keyboard;
					keyboard.usUsagePage = 0x01; //Generic Desktop Controls
					keyboard.usUsage = 0x06; //Keyboard
					keyboard.dwFlags = RIDEV_INPUTSINK;
					keyboard.hwndTarget = dynamic_cast<Platform::Window*>(Window)->GetHWND();
					if (!RegisterRawInputDevices(&keyboard, 1, sizeof(RAWINPUTDEVICE))) {
						return;
					}
					
					//Now we've registered keyboard input from Windows, we can figure out what
					//keyboards to expect.
					//Let's get the list of all devices.
					unsigned int num_devices;
					GetRawInputDeviceList(nullptr, &num_devices, sizeof(RAWINPUTDEVICELIST));
					RAWINPUTDEVICELIST* list = new RAWINPUTDEVICELIST[num_devices];
					GetRawInputDeviceList(list, &num_devices, sizeof(RAWINPUTDEVICELIST));

					//For each device found, we need to look for all keyboards
					bool found_first = false;
					for (unsigned int i = 0; i < num_devices; i++) {
						RAWINPUTDEVICELIST& device = list[i];
						if (device.dwType == RIM_TYPEKEYBOARD) {
							//Now we know the handle, we can look for other info
							HANDLE handle = device.hDevice;
							RID_DEVICE_INFO info; info.cbSize = sizeof(RID_DEVICE_INFO);
							GetRawInputDeviceInfo(handle, RIDI_DEVICEINFO, &info, (PUINT)&info.cbSize);

							//Then we can create a Keyboard object
							Keyboard* keyboard = new Keyboard;
							keyboard->device_handle = handle;

							//And stow it.
							size_t index = Keyboard::all_keyboard_devices.size();
							Keyboard::all_keyboard_devices.push_back(keyboard);
							Keyboard::device_map[handle] = index;

							//If this is our first keyboard, let's set it as our primary.
							if (!found_first) {
								found_first = true;
								primary = keyboard;
							}
						}
					}
				}
				static void Shutdown() {
					for (auto* keyboard : all_keyboard_devices) {
						delete keyboard;
					}
					all_keyboard_devices.clear();
				}
			};
		}
	}
}

#endif