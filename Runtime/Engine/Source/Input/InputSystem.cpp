#include "Input/InputSystem.h"

using namespace Blueshift;
using namespace Input;

InputSystem::InputSystem() 
	: KeyboardEvents(16),
	  MouseEvents(16) {

}

InputSystem::~InputSystem() {

}

void InputSystem::PollDevices() {
	//Read all we can from each of our input queues.

	KeyboardEvent* kb;
	MouseEvent* ms;

	while ((kb = KeyboardEvents.Read()) != nullptr) {
		//Translate the keyboard event into something usable

	}

	while ((ms = MouseEvents.Read()) != nullptr) {
		//Translate the mouse event into something usable

	}
}