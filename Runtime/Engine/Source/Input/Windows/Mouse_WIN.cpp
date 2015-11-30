#ifdef _WIN32
#include "Input/Windows/Mouse_WIN.h"
#include "Platform/Windows/Window_WIN.h"

using namespace Blueshift;
using namespace Blueshift::Input;
using namespace Blueshift::Input::Devices;

std::vector<Mouse*> Mouse::all_mouse_devices;
std::map<HANDLE, size_t> Mouse::device_map;

Mouse* Mouse::primary = nullptr;
bool Mouse::primary_found = false;

Mouse::Mouse() {
	
}

Mouse::~Mouse() {

}

bool Mouse::IsButtonDown(ButtonName Button) {
	if (Button > ButtonName::MouseMax) {
		return false;
	}
	return buttons[(size_t)Button];
}

void Mouse::Poll() {
	polled_translation = translation_accumulator;
	translation_accumulator *= 0.0;
}

void Mouse::_pass_event(RAWMOUSE* data) {
	//Update the mouse state from the data
	float wheel_delta = 0.0;
	if (data->usButtonFlags == RI_MOUSE_WHEEL) {
		wheel_delta = (float)data->usButtonData;
	}
	translation_accumulator += Core::Math::Vector4((float)data->lLastX, (float)data->lLastY, wheel_delta, 0.0f);
	
	switch (data->usButtonFlags) {
	case RI_MOUSE_LEFT_BUTTON_DOWN:
		buttons[(size_t)ButtonName::MouseLeft] = true;
		break;
	case RI_MOUSE_LEFT_BUTTON_UP:
		buttons[(size_t)ButtonName::MouseLeft] = false;
		break;
	case RI_MOUSE_RIGHT_BUTTON_DOWN:
		buttons[(size_t)ButtonName::MouseRight] = true;
		break;
	case RI_MOUSE_RIGHT_BUTTON_UP:
		buttons[(size_t)ButtonName::MouseRight] = false;
		break;
	case RI_MOUSE_MIDDLE_BUTTON_DOWN:
		buttons[(size_t)ButtonName::MouseMiddle] = true;
		break;
	case RI_MOUSE_MIDDLE_BUTTON_UP:
		buttons[(size_t)ButtonName::MouseMiddle] = false;
		break;
	}
}

#endif