#ifdef _WIN32 //Windows platform
#include "Input/Windows/Keyboard_WIN.h"
#include "Platform/Windows/Window_WIN.h"
#include <map>

using namespace Blueshift;
using namespace Input;
using namespace Devices;
using std::map;

std::vector<Keyboard*> Keyboard::all_keyboard_devices;
std::map<HANDLE, size_t> Keyboard::device_map;

Keyboard* Keyboard::primary = nullptr;
bool Keyboard::primary_found = false;

Core::Event<char> Keyboard::TextInput;

//VKey to ButtonName translation
std::map<USHORT, ButtonName> TranslationTable = {
	{ VK_ESCAPE,	ButtonName::Esc },
	{ VK_RETURN,	ButtonName::Return },
	{ VK_TAB,		ButtonName::Tab },
	{ VK_SPACE,		ButtonName::Space },
	{ VK_BACK,		ButtonName::Backspace },
	
	{ VK_UP,		ButtonName::Up },
	{ VK_DOWN,		ButtonName::Down },
	{ VK_LEFT,		ButtonName::Left },
	{ VK_RIGHT,		ButtonName::Right },

	{ VK_INSERT,	ButtonName::Insert },
	{ VK_DELETE,	ButtonName::Delete },
	{ VK_HOME,		ButtonName::Home },
	{ VK_END,		ButtonName::End },

	{ VK_PRIOR,		ButtonName::PageUp },
	{ VK_NEXT,		ButtonName::PageDown },

	{ VK_SNAPSHOT,	ButtonName::PrintScreen },

	{ VK_OEM_PLUS,	ButtonName::Plus },
	{ VK_OEM_MINUS,	ButtonName::Minus },

	{ VK_OEM_4,		ButtonName::LeftBracket },
	{ VK_OEM_6,		ButtonName::RightBracket },

	{ VK_OEM_1,		ButtonName::Semicolon },
	{ VK_OEM_7,		ButtonName::Quote },
	{ VK_OEM_COMMA,	ButtonName::Comma },
	{ VK_OEM_PERIOD,ButtonName::Period },

	{ VK_OEM_2,		ButtonName::Slash },
	{ VK_OEM_5,		ButtonName::Backslash },
	{ VK_OEM_3,		ButtonName::Tilde },
	{ VK_SEPARATOR,	ButtonName::Pipe },

	{ VK_F1,		ButtonName::F1 },
	{ VK_F2,		ButtonName::F2 },
	{ VK_F3,		ButtonName::F3 },
	{ VK_F4,		ButtonName::F4 },
	{ VK_F5,		ButtonName::F5 },
	{ VK_F6,		ButtonName::F6 },
	{ VK_F7,		ButtonName::F7 },
	{ VK_F8,		ButtonName::F8 },
	{ VK_F9,		ButtonName::F9 },
	{ VK_F10,		ButtonName::F10 },
	{ VK_F11,		ButtonName::F11 },
	{ VK_F12,		ButtonName::F12 },

	{ VK_NUMPAD0,	ButtonName::NumPad0 },
	{ VK_NUMPAD1,	ButtonName::NumPad1 },
	{ VK_NUMPAD2,	ButtonName::NumPad2 },
	{ VK_NUMPAD3,	ButtonName::NumPad3 },
	{ VK_NUMPAD4,	ButtonName::NumPad4 },
	{ VK_NUMPAD5,	ButtonName::NumPad5 },
	{ VK_NUMPAD6,	ButtonName::NumPad6 },
	{ VK_NUMPAD7,	ButtonName::NumPad7 },
	{ VK_NUMPAD8,	ButtonName::NumPad8 },
	{ VK_NUMPAD9,	ButtonName::NumPad9 },

	{ uint8_t('0'), ButtonName::Key0 },
	{ uint8_t('1'), ButtonName::Key1 },
	{ uint8_t('2'), ButtonName::Key2 },
	{ uint8_t('3'), ButtonName::Key3 },
	{ uint8_t('4'), ButtonName::Key4 },
	{ uint8_t('5'), ButtonName::Key5 },
	{ uint8_t('6'), ButtonName::Key6 },
	{ uint8_t('7'), ButtonName::Key7 },
	{ uint8_t('8'), ButtonName::Key8 },
	{ uint8_t('9'), ButtonName::Key9 },

	{ uint8_t('A'), ButtonName::KeyA },
	{ uint8_t('B'), ButtonName::KeyB },
	{ uint8_t('C'), ButtonName::KeyC },
	{ uint8_t('D'), ButtonName::KeyD },
	{ uint8_t('E'), ButtonName::KeyE },
	{ uint8_t('F'), ButtonName::KeyF },
	{ uint8_t('G'), ButtonName::KeyG },
	{ uint8_t('H'), ButtonName::KeyH },
	{ uint8_t('I'), ButtonName::KeyI },
	{ uint8_t('J'), ButtonName::KeyJ },
	{ uint8_t('K'), ButtonName::KeyK },
	{ uint8_t('L'), ButtonName::KeyL },
	{ uint8_t('M'), ButtonName::KeyM },
	{ uint8_t('N'), ButtonName::KeyN },
	{ uint8_t('O'), ButtonName::KeyO },
	{ uint8_t('P'), ButtonName::KeyP },
	{ uint8_t('Q'), ButtonName::KeyQ },
	{ uint8_t('R'), ButtonName::KeyR },
	{ uint8_t('S'), ButtonName::KeyS },
	{ uint8_t('T'), ButtonName::KeyT },
	{ uint8_t('U'), ButtonName::KeyU },
	{ uint8_t('V'), ButtonName::KeyV },
	{ uint8_t('W'), ButtonName::KeyW },
	{ uint8_t('X'), ButtonName::KeyX },
	{ uint8_t('Y'), ButtonName::KeyY },
	{ uint8_t('Z'), ButtonName::KeyZ },
};

Keyboard::Keyboard() {

}

Keyboard::~Keyboard() {

}

bool Keyboard::IsButtonDown(ButtonName Button) {
	return matrix[(size_t)Button];
}

bool Keyboard::WasButtonPressed(ButtonName Button) {
	return matrix[(size_t)Button] && !last_matrix[(size_t)Button];
}

bool Keyboard::WasButtonReleased(ButtonName Button) {
	return !matrix[(size_t)Button] && last_matrix[(size_t)Button];
}

void Keyboard::Poll() {
	memcpy(&last_matrix[0], &matrix[0], sizeof(bool) * (size_t)ButtonName::Max);
}

void Keyboard::_pass_event(RAWKEYBOARD* data) {
	//But wait! We're getting keyboard input?
	//It could be our primary!
	if (!Keyboard::primary_found) {
		Keyboard::primary = this;
		Keyboard::primary_found = true;
	}

	bool key_down = true;
	if (data->Flags & RI_KEY_BREAK) {
		key_down = false;
	}

	USHORT key = data->VKey;
	if (key == VK_CONTROL) {
		key = data->Flags & RI_KEY_E0 ? VK_RCONTROL : VK_LCONTROL;
	}
	//Convert VKey to ButtonName
	ButtonName button = TranslationTable[data->VKey];
	matrix[(size_t)button] = key_down;
}

#endif