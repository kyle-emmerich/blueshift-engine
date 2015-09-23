#include "Input/Devices/Keyboard.h"

using namespace Blueshift;
using namespace Input;
using namespace Devices;

bool Keyboard::IsButtonDown(ButtonName Button) {
	return matrix[(size_t)Button];
}

Keyboard* Keyboard::Open() {
	return nullptr;
}

void Keyboard::Close(Keyboard *) {

}

