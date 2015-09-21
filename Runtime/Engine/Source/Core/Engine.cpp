#include "Engine.h"
#include "bgfx/bgfx.h"
#include "Graphics/VertexDeclarations.h"
#include <vector>

Engine* Engine::_instance = 0;

Engine::Engine() {
	DisplayInfo::EnumerateDisplays(this);
	primary_display_index = 0;

	Engine::_instance = this;
}

Engine::~Engine() {
	if (render_thread.joinable()) {
		render_thread.join();
	}
}

void Engine::AddAvailableDisplayInfo(DisplayInfo&& Display) {
	Display.Index = (uint32_t)displays.GetSize();
	displays.Add(Display);
}

DisplayInfo& Engine::GetPrimaryDisplay() const {
	return displays[primary_display_index];
}
DisplayInfo& Engine::GetDisplay(size_t index) const {
	return displays[index];
}
const Array<DisplayInfo>& Engine::GetDisplays() const {
	return displays;
}

void Engine::SetPrimaryDisplay(const DisplayInfo& display) {
	primary_display_index = display.Index;
}
void Engine::SetPrimaryDisplay(size_t index) {
	primary_display_index = index;
}

void Engine::AddRenderWindow(RenderWindow& Window) {
	render_windows.Add(&Window);
}

RenderWindow& Engine::GetPrimaryRenderWindow() const {
	return *render_windows[0];
}

const Array<RenderWindow*>& Engine::GetRenderWindows() const {
	return render_windows;
}

int Engine::Run() {
	
	RenderWindow window(1280, 720);
	window.SetTitle("Blueshift Engine");
	//window.SetFullscreenDesktop(true, true);
	AddRenderWindow(window);

	render_thread = std::thread(&Engine::render_thread_func, this);
	while (window.ProcessEvents()) {

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
		

		for (RenderWindow* window : render_windows) {
			window->PreRender();
			window->PostRender();
		}

		bgfx::frame();
	}

	bgfx::shutdown();
}