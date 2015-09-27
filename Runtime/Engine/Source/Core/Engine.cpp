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

Engine::Engine() {
	DisplayInfo::EnumerateDisplays(this);
	primary_display_index = 0;

	Engine::_instance = this;
}

Engine::~Engine() {
	for (auto* window : render_windows) {
		if (window != nullptr) {
			window->Close();
			delete window;
		}
	}
	render_windows.clear();
	if (render_thread.joinable()) {
		render_thread.join();
	}
}

void Engine::AddAvailableDisplayInfo(DisplayInfo&& Display) {
	Display.Index = (uint32_t)displays.size();
	displays.push_back(Display);
}

const DisplayInfo& Engine::GetPrimaryDisplay() const {
	return displays[primary_display_index];
}
const DisplayInfo& Engine::GetDisplay(size_t index) const {
	return displays[index];
}
const std::vector<DisplayInfo>& Engine::GetDisplays() const {
	return displays;
}

void Engine::SetPrimaryDisplay(const DisplayInfo& display) {
	primary_display_index = display.Index;
}
void Engine::SetPrimaryDisplay(size_t index) {
	primary_display_index = index;
}

void Engine::AddRenderWindow(RenderWindow* Window) {
	render_windows.push_back(Window);
}

RenderWindow& Engine::GetPrimaryRenderWindow() const {
	return *render_windows[0];
}

const std::vector<RenderWindow*>& Engine::GetRenderWindows() const {
	return render_windows;
}

int Engine::Run() {
	ApplicationConfig.ReadFile("application.cfg");
	RenderWindow* window = new RenderWindow(
		ApplicationConfig.Get<uint32_t>("Window", "Width", 1280),
		ApplicationConfig.Get<uint32_t>("Window", "Height", 720)
		);
	window->SetTitle("Blueshift Engine");

	if (ApplicationConfig.Get<bool>("Window", "Fullscreen", false)) {
		window->SetFullscreen(true);
	}
	if (ApplicationConfig.Get<bool>("Window", "FullscreenDesktop", false)) {
		window->SetFullscreenDesktop(true, ApplicationConfig.Get<bool>("Window", "SpanAllDisplays", false));
	}
	AddRenderWindow(window);

	Input::Devices::Keyboard::Register(window);
	Input::Devices::Mouse::Register(window);
	Input::Devices::Joystick::Register(window);

	render_thread = std::thread(&Engine::render_thread_func, this);
	while (window->ProcessEvents()) {

	}
	stop = true;

	return 0;
}

void Engine::render_thread_func() {
	bgfx::init();
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
					   0x303030ff, 1.0f, 0);

	bgfx::setDebug(BGFX_DEBUG_TEXT);

	InitializeVertexDeclarations();

	while (!stop) {
		if (Input::Devices::Mouse::Primary()->IsButtonDown(Input::ButtonName::MouseLeft)) {
			bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
				0x909090ff, 1.0f, 0);
		}
		else {
			bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
				0x303030ff, 1.0f, 0);
		}

		for (RenderWindow* window : render_windows) {
			window->PreRender();
			window->PostRender();
		}

		bgfx::frame();
	}

	bgfx::shutdown();
}