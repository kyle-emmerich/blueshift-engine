#include "Core/Core.h"
#include "Core/Engine.h"

int main(int argc, char* argv[]) {
	Blueshift::Core::Engine engine;
	engine.SetPrimaryDisplay(1);

	return engine.Run();
} 