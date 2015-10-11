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

Engine::Engine(bool WithRendering) {
	Engine::_instance = this;

	if (WithRendering) {
		render = new RenderSystem;
	}
}

Engine::~Engine() {
	
}



int Engine::Run() {
	if (render != nullptr) {

	}
	return 0;
}

