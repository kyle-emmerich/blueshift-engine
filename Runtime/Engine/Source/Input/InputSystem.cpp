#include "Input/InputSystem.h"
#include "Core/Engine.h"

using namespace Blueshift;
using namespace Input;

InputSystem::InputSystem(Blueshift::Core::Engine* Engine) 
	: keyboard_events(256), mouse_events(256) {

}

InputSystem::~InputSystem() {

}

void InputSystem::ReportKeyboardEvent(KeyboardEvent&& ev) {
	KeyboardEvent* kb_ev = keyboard_events.Request();
	*kb_ev = std::move(ev);
}

void InputSystem::ReportMouseEvent(MouseEvent&& ev) {
	MouseEvent* ms_ev = mouse_events.Request();
	*ms_ev = std::move(ev);
}

void InputSystem::PollDevices() {
	//Read all we can from each of our input queues.

	KeyboardEvent* kb;
	MouseEvent* ms;

	while ((kb = keyboard_events.Pop()) != nullptr) {
		//Translate the keyboard event into something usable
		if (kb->is_down) {
			Core::Engine::Get().Log(Core::LogLevel::Notice, Formatter() << "Pressed key " << kb->name);
		} else {
			Core::Engine::Get().Log(Core::LogLevel::Notice, Formatter() << "Released key " << kb->name);
		}
	}

	while ((ms = mouse_events.Pop()) != nullptr) {
		//Translate the mouse event into something usable
		if (ms->type == MouseEvent::Type::StateUpdate)
			Core::Engine::Get().Log(Core::LogLevel::Notice, Formatter() << "Mouse moved by " << ms->x << ", " << ms->y);
	}
}