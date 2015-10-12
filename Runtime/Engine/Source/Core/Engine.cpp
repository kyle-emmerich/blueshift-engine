#include "Engine.h"
#include "bgfx/bgfx.h"
#include "Graphics/VertexDeclarations.h"
#include "Platform/PlatformSpecific.h"
#include <vector>
#include <iostream>

using namespace Blueshift;
using namespace Blueshift::Core;
using namespace Blueshift::Graphics;
using namespace Blueshift::Platform;

Engine* Engine::_instance = 0;

Engine::Engine(EngineParameters* Parameters) {
	Engine::_instance = this;

	parameters = Parameters;

	if (parameters->Subsystems.Rendering) {
		render = new RenderSystem;
	}
}

Engine::~Engine() {
	delete parameters;
}
