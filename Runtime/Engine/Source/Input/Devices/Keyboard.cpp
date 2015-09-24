#include "Input/Devices/Keyboard.h"

using namespace Blueshift;
using namespace Input;
using namespace Devices;

Keyboard::Keyboard() {

}

Keyboard::~Keyboard() {

}

bool Keyboard::IsButtonDown(ButtonName Button) {
	return matrix[(size_t)Button];
}