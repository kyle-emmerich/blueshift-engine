#pragma once
#include "Core/Core.h"
#include "Core/ISubsystem.h"
#include "Core/EventQueue.h"

namespace Blueshift {
	namespace Input {
		enum class Button {
			None = 0,

			MouseLeft, MouseRight, MouseMiddle,
			MouseMax,

			CapsLock,
			Esc, Enter, Tab, Space, Backspace, Up, Down, Left, Right,
			Insert, Delete, Home, End, PageUp, PageDown,
			PrintScreen, Plus, Minus, LeftBracket, RightBracket, Semicolon, Quote, Comma, Period,
			Slash, Backslash, Tilde, Pipe,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			NumPad0, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7, NumPad8, NumPad9,
			NumLock, NumPadEnter, NumPadDecimal,
			Multiply, Divide, Add, Subtract,
			Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9,
			KeyA, KeyB, KeyC, KeyD, KeyE, KeyF, KeyG, KeyH, KeyI, KeyJ, KeyK, KeyL, KeyM, 
			KeyN, KeyO, KeyP, KeyQ, KeyR, KeyS, KeyT, KeyU, KeyV, KeyW, KeyX, KeyY, KeyZ,
			LeftControl, RightControl, LeftAlt, RightAlt, LeftShift, RightShift,
			KeyboardMax,

			GamepadA, GamepadB, GamepadX, GamepadY, GamepadThumbL, GamepadThumbR,
			GamepadShoulderL, GamepadShoulderR, GamepadUp, GamepadDown,
			GamepadLeft, GamepadRight, GamepadBack, GamepadStart,
			GamepadGuide, 
			GamepadMax,

			PenErase, PenSpecial0, PenSpecial1, PenSpecial2, 
			PenMax,

			Max
		};

		struct KeyboardEvent {
			bool is_down;
			Button key;
			char name[16];
		};

		struct MouseEvent {
			enum class Type {
				MoveTo,
				StateUpdate
			};
			Type type;
			int x;
			int y;
			bool buttons[(size_t)Button::MouseMax];
			int scroll_amount;
		};

		class InputSystem : public Core::ISubsystem {
		private:

		public:
			Core::EventQueue<KeyboardEvent> KeyboardEvents;
			Core::EventQueue<MouseEvent> MouseEvents;

			 InputSystem();
			~InputSystem();

			void PollDevices();
		};
	}
}