#include "Core/Core.h"
#include "Core/Engine.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Matrix.h"

#include <functional>
#include <iostream>

#include "Core/Event.h"

struct TestEvent {
	int x = 5;
};
void test_subscriber(const TestEvent& ev) {
	std::cout << ev.x << std::endl;
}

int main(int argc, char* argv[]) {
	Blueshift::Core::Engine engine;
	engine.SetPrimaryDisplay(1);

	return engine.Run();
} 