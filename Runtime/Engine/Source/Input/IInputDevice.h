#pragma once

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

		typedef size_t AxisHandle;

		class IInputDeviceCapability;
		class IInputDeviceCapabilityAxes {
		public:


			virtual double GetAxis(AxisHandle Axis) = 0;
		};
		class IInputDeviceCapabilityButtons {
		public:

		};
		class IInputDeviceCapabilityHapticFeedback {
		public:

		};
		class IInputDeviceCapabilityPositional {
		public:

		};

		class IInputDevice {
		public:

		};

	}
}