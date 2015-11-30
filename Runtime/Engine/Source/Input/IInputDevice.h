#pragma once
#ifdef _WIN32 //Windows platform

#include <string>
#include "Core/Event.h"
#include "Core/Math/Vector.h"
#include "Platform/IWindow.h"

namespace Blueshift {
	namespace Input {

		enum class InputDeviceType {
			Keyboard,
			Mouse,
			Joystick,
			Yoke,
			Gamepad,

			SteeringWheel,
			Touchscreen,
			StylusPen,

			Microphone,
			Camera,

			Accelerometer,
			Gyroscope,
			Magnetometer,

			MicrosoftKinect,

			TrackIR,
			OculusRift,
			GenericVR
		};

		enum class InputDeviceCapability {
			Axes,
			Buttons,
			HapticFeedback,
			Positional,
			Translational,
		};

		enum class ButtonName : size_t {
			None = 0,
			MouseLeft, MouseRight, MouseMiddle,
			MouseMax,

			Esc, Return, Tab, Space, Backspace,
			
			Up, Down, Left, Right,
			
			Insert, Delete, Home, End,
			PageUp, PageDown,
			
			PrintScreen, 
			
			Plus, Minus,
			LeftBracket, RightBracket,
			Semicolon, Quote, Comma, Period,
			Slash, Backslash, Tilde, Pipe,
			
			F1, F2, F3, F4, F5, F6,
			F7, F8, F9, F10, F11, F12,
			
			NumPad0, NumPad1, NumPad2,
			NumPad3, NumPad4, NumPad5,
			NumPad6, NumPad7, NumPad8,
			NumPad9,
			
			Key0, Key1, Key2, Key3, Key4,
			Key5, Key6, Key7, Key8, Key9,
			
			KeyA,
			KeyB,
			KeyC,
			KeyD,
			KeyE,
			KeyF,
			KeyG,
			KeyH,
			KeyI,
			KeyJ,
			KeyK,
			KeyL,
			KeyM,
			KeyN,
			KeyO,
			KeyP,
			KeyQ,
			KeyR,
			KeyS,
			KeyT,
			KeyU,
			KeyV,
			KeyW,
			KeyX,
			KeyY,
			KeyZ,

			LeftControl, RightControl,
			LeftAlt, RightAlt,
			LeftShift, RightShift,

			KeyboardMax,

			GamepadA, GamepadB, GamepadX, GamepadY,
			GamepadThumbL, GamepadThumbR,
			GamepadShoulderL, GamepadShoulderR,
			GamepadUp, GamepadDown,
			GamepadLeft, GamepadRight,
			GamepadBack, GamepadStart,
			GamepadGuide,
			GamepadMax,

			PenErase,
			PenSpecial0,
			PenSpecial1,
			PenSpecial2,
			PenMax,

			Max
		};

		typedef size_t AxisHandle;
		struct AxisDescription {
			int offset;
			double scale;

			int raw_value;
			double value;

			double delta;
			double deadzone = 0.05;
		};

		class IInputDevice {
		public:
			virtual std::string GetName() = 0;
			virtual void Poll() = 0;
		};

		namespace DeviceCapability {
			class Axes {
			public:
				virtual double GetAxis(AxisHandle Axis, bool ignore_deadzone = false) const = 0;
				virtual const AxisDescription& GetAxisDescription(AxisHandle Axis) const = 0;
			};
			class Buttons {
			public:
				virtual bool IsButtonDown(ButtonName Button) = 0;
			};
			class HapticFeedback {
			public:

			};

			template<size_t n = 1>
			class Positional {
			public:
				virtual Core::Math::Vector4 GetPosition(size_t index = 0) = 0;
			};

			template<size_t n = 1>
			class Translational {
			public:
				virtual Core::Math::Vector4 GetDelta(size_t index = 0) = 0;
			};
		}
	}
}

#endif