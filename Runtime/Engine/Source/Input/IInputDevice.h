#pragma once
#include <string>
#include "Core/Event.h"
#include "Core/Math/Vector.h"

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
		};

		enum class ButtonName : size_t {
			None = 0,
			MouseLeft, MouseRight, MouseMiddle,

			Esc, Return, Tab, Space, Backspace,
			
			Up, Down, Left, Right,
			
			Insert, Delete, Home, End,
			PageUp, PageDown,
			
			Print, 
			
			Plus, Minus,
			LeftBracket, RightBracket,
			Semicolon, Quote, Comma, Period,
			Slash, Backslash, Tilde,
			
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

			GamepadA, GamepadB, GamepadX, GamepadY,
			GamepadThumbL, GamepadThumbR,
			GamepadShoulderL, GamepadShoulderR,
			GamepadUp, GamepadDown,
			GamepadLeft, GamepadRight,
			GamepadBack, GamepadStart,
			GamepadGuide,

			PenErase,
			PenSpecial0,
			PenSpecial1,
			PenSpecial2,

			Max
		};

		typedef size_t AxisHandle;

		class IInputDeviceCapability;
		class IInputDeviceCapabilityAxes {
		public:
			virtual double GetAxis(AxisHandle Axis) = 0;

			virtual std::string GetAxisName(AxisHandle Axis) = 0;
			virtual AxisHandle GetAxisHandle(std::string AxisName) = 0;
		};
		class IInputDeviceCapabilityButtons {
		public:
			virtual bool IsButtonDown(ButtonName Button) = 0;
			
			Core::Event<void(IInputDevice&, ButtonName)> ButtonPressed;
			Core::Event<void(IInputDevice&, ButtonName)> ButtonReleased;
		};
		class IInputDeviceCapabilityHapticFeedback {
		public:

		};

		template<size_t n, size_t dimensions>
		class IInputDeviceCapabilityPositional {
		public:
			virtual Core::Math::Vector<dimensions, double> GetPosition(size_t index = 0) = 0;
		};

		class IInputDevice {
		public:
			virtual std::string GetName() = 0;
		};

		template<typename T>
		T* OpenDevice() {
			//non-functional
			return nullptr;
		}

		template<typename T>
		void CloseDevice(T* Device) {
			//non-functional
		}

	}
}