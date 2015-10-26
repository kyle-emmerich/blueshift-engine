#pragma once
#ifdef _WIN32 //Windows platform
#include "Input/IInputDevice.h"
#include "Platform/IWindow.h"
#include "Platform/Windows/Window_WIN.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <map>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace Blueshift {
	namespace Input {
		namespace Devices {
			BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE*, VOID*);

			class Joystick
				: public IInputDevice,
				  public DeviceCapability::Axes,
				  public DeviceCapability::Buttons,
				  public DeviceCapability::HapticFeedback {
			protected:
				static std::vector<Joystick*> all_joystick_devices;
				static std::map<GUID, size_t> device_map;

				static Joystick* primary;
				static bool primary_found;

				static LPDIRECTINPUT8 dinput;
				LPDIRECTINPUTDEVICE8 input_device;
				GUID guid;

				std::string name;
				
				std::vector<AxisDescription> axes;

				
			public:
				static Joystick* Primary() { return primary; }

				//Implement axes
				virtual double GetAxis(AxisHandle Axis, bool ignore_deadzone = false) const;
				virtual const AxisDescription& GetAxisDescription(AxisHandle Axis) const {
					return axes[Axis];
				}
				virtual size_t NumAxes() const {
					return axes.size();
				}

				//Implement buttons
				virtual bool IsButtonDown(ButtonName Button) { return false; }

				//Implement device
				virtual std::string GetName() { return "Joystick"; }
				virtual void Poll();

				static void Register(Platform::IWindow* Window);
				static void Shutdown();

				virtual void initialize(const DIDEVICEINSTANCE* instance, Platform::IWindow* window);
			};

		}
	}
}
#endif