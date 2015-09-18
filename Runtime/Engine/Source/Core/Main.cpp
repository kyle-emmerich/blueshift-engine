#include "Core/Core.h"
#include "Core/Engine.h"
#include "Core/Math/Vector.h"

#include <iostream>

int main(int argc, char* argv[]) {
	Engine engine;
	engine.SetPrimaryDisplay(1);

	Vector3 x(1.0, 2.0, 500.0);
	std::cout<<x.Length()<<std::endl;

	


	return engine.Run();
} 