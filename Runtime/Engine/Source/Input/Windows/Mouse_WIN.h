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

			class Mouse 
				: public IInputDevice, 
				  public DeviceCapability::Buttons, 
				  public DeviceCapability::Positional<2>,
				  public DeviceCapability::Translational<3> {
			protected:
				static std::vector<Mouse*> all_mouse_devices;
				static std::map<HANDLE, size_t> device_map;

				static Mouse* primary;
				static bool primary_found;

				bool buttons[(size_t)ButtonName::MouseMax];

				Core::Math::Vector3 translation_accumulator;
				Core::Math::Vector3 polled_translation;

				Core::Math::Vector2 last_position;

				HANDLE device_handle;
			public:
				Mouse();
				~Mouse();

				static Mouse* Primary() { return primary; }

				//Implement buttons capability
				virtual bool IsButtonDown(ButtonName Button);

				//Implement positional capability
				virtual Core::Math::Vector2 GetPosition(size_t index = 0) { return last_position; }

				//Implement translational capability
				virtual Core::Math::Vector3 GetDelta(size_t index = 0) { return polled_translation; }

				//Implement device
				virtual std::string GetName() { return "Mouse"; }
				virtual void Poll();

				//Windows implementation
				static Mouse* _get_from_handle(HANDLE handle) {
					return device_map.find(handle) != device_map.end() ? all_mouse_devices[device_map[handle]] : nullptr;
				}
				static void _set_last_position(long x, long y) {
					for (Mouse* mouse : all_mouse_devices) {
						mouse->last_position.X = (double)x;
						mouse->last_position.Y = (double)y;
					}
				}
				void _pass_event(RAWMOUSE* data);

				static void Register(Platform::IWindow* Window) {
					//On Windows, this means we register USB devices that
					//are on generic desktop controls and are considered mice.
					RAWINPUTDEVICE mouse;
					mouse.usUsagePage = 0x01; //Generic Desktop Controls
					mouse.usUsage = 0x02; //Mouse
					mouse.dwFlags = RIDEV_INPUTSINK;
					mouse.hwndTarget = dynamic_cast<Platform::Window*>(Window)->GetHWND();
					if (!RegisterRawInputDevices(&mouse, 1, sizeof(RAWINPUTDEVICE))) {
						return;
					}

					//Now we've registered mouse input from Windows, we can figure out what
					//mice to expect.
					//Let's get the list of all devices.
					unsigned int num_devices;
					GetRawInputDeviceList(nullptr, &num_devices, sizeof(RAWINPUTDEVICELIST));
					RAWINPUTDEVICELIST* list = new RAWINPUTDEVICELIST[num_devices];
					GetRawInputDeviceList(list, &num_devices, sizeof(RAWINPUTDEVICELIST));

					//For each device found, we need to look for all mouse devices
					bool found_first = false;
					for (unsigned int i = 0; i < num_devices; i++) {
						RAWINPUTDEVICELIST& device = list[i];
						if (device.dwType == RIM_TYPEMOUSE) {
							//Now we know the handle, we can look for other info
							HANDLE handle = device.hDevice;
							RID_DEVICE_INFO info; info.cbSize = sizeof(RID_DEVICE_INFO);
							GetRawInputDeviceInfo(handle, RIDI_DEVICEINFO, &info, (PUINT)&info.cbSize);

							//Then we can create a Mouse object
							Mouse* mouse = new Mouse;
							mouse->device_handle = handle;

							//And stow it.
							size_t index = Mouse::all_mouse_devices.size();
							Mouse::all_mouse_devices.push_back(mouse);
							Mouse::device_map[handle] = index;

							//If this is our first mouse, let's set it as our primary.
							if (!found_first) {
								found_first = true;
								primary = mouse;
							}
						}
					}
				}
				static void Shutdown() {
					for (auto* mouse : all_mouse_devices) {
						delete mouse;
					}
					all_mouse_devices.clear();
				}

				static void _pass_all(RAWMOUSE* data) {
					for (auto* mouse : all_mouse_devices) {
						mouse->_pass_event(data);
					}
				}
			};

		}
	}
}

#endif