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

			class Joystick
				: public IInputDevice,
				  public DeviceCapability::Axes,
				  public DeviceCapability::Buttons,
				  public DeviceCapability::HapticFeedback {
			protected:
				static std::vector<Joystick*> all_joystick_devices;
				static std::map<HANDLE, size_t> device_map;

				static Joystick* primary;
				static bool primary_found;

				UINT id;

				JOYINFOEX info;
				JOYCAPS caps;
				std::vector<AxisDescription> axes;
				UINT num_axes;
			public:
				static Joystick* Primary() { return primary; }

				//Implement axes
				virtual double GetAxis(AxisHandle Axis, bool ignore_deadzone = false) const;
				virtual const AxisDescription& GetAxisDescription(AxisHandle Axis) const {
					return axes[Axis];
				}
				virtual size_t NumAxes() const {
					return num_axes;
				}

				//Implement buttons
				virtual bool IsButtonDown(ButtonName Button) { return false; }

				//Implement device
				virtual std::string GetName() { return "Joystick"; }
				virtual void Poll();

				virtual void Initialize();

				static void Register(Platform::IWindow* Window) {
					//Try all the joysticks!
					UINT max_joysticks = joyGetNumDevs();
					JOYINFOEX info;
					JOYCAPS caps;
					bool found_first = false;
					for (UINT i = 0; i < max_joysticks; i++) {
						info.dwSize = sizeof(JOYINFOEX);
						info.dwFlags = JOY_RETURNALL;
						if (joyGetPosEx(i, &info) == JOYERR_NOERROR) {
							if (joyGetDevCaps(i, &caps, sizeof(JOYCAPS)) == JOYERR_NOERROR) {
								Joystick* js = new Joystick;
								js->info = info;
								js->caps = caps;
								js->Initialize();

								if (!found_first) {
									found_first = true;
									primary = js;
								}
							}
						}
					}
				}
				static void Shutdown() {
					for (auto* joystick : all_joystick_devices) {
						delete joystick;
					}
					all_joystick_devices.clear();
				}
			};

		}
	}
}
#endif