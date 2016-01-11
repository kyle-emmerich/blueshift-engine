#include "Input/InputSystem.h"
#include "Core/Engine.h"

using namespace Blueshift;
using namespace Input;

InputSystem::InputSystem(Blueshift::Core::Engine* Engine) 
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
		if (kb->is_down) {
			Core::Engine::Get().Log(Core::LogLevel::Notice, Formatter() << "Pressed key " << kb->name);
		} else {
			Core::Engine::Get().Log(Core::LogLevel::Notice, Formatter() << "Released key " << kb->name);
		}
	}

	while ((ms = MouseEvents.Read()) != nullptr) {
		//Translate the mouse event into something usable

	}
}