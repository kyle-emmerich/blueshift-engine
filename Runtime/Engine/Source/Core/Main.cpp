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
	Engine engine;
	engine.SetPrimaryDisplay(1);

	Vector3 x(1.0, 2.0, 500.0);
	std::cout<<x.Length()<<std::endl;

	Vector3 result = x + Vector3(100.0, 20.0, 200.0);
	std::cout << result << std::endl;
	
	Matrix4 m {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	std::cout << m << std::endl;
	Matrix4 m2 {
		10.0, 10.0, 0.0, 10.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	Matrix4 m3 = m * m2;
	std::cout << m3 << std::endl;

	std::cout << Determinant(m2) << std::endl;

	Event<TestEvent> ev;
	ev.Subscribe(test_subscriber);
	
	TestEvent event_obj;
	event_obj.x = 50515;
	ev(event_obj);
	event_obj.x = 47578;
	ev(event_obj);
	 
	return engine.Run();
} 