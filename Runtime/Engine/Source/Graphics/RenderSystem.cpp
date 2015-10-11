#include "Graphics/RenderSystem.h"
#include "Graphics/VertexDeclarations.h"
#include "Core/Engine.h"

using namespace Blueshift;
using namespace Blueshift::Graphics;
using namespace Blueshift::Platform;

RenderSystem::RenderSystem() {
	DisplayInfo::EnumerateDisplays(this);
	primary_display_index = 0;
}

RenderSystem::~RenderSystem() {
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

void RenderSystem::AddAvailableDisplayInfo(DisplayInfo&& Display) {
	Display.Index = (uint32_t)displays.size();
	displays.push_back(Display);
}

const DisplayInfo& RenderSystem::GetPrimaryDisplay() const {
	return displays[primary_display_index];
}
const DisplayInfo& RenderSystem::GetDisplay(size_t index) const {
	return displays[index];
}
const std::vector<DisplayInfo>& RenderSystem::GetDisplays() const {
	return displays;
}

void RenderSystem::SetPrimaryDisplay(const DisplayInfo& display) {
	primary_display_index = display.Index;
}
void RenderSystem::SetPrimaryDisplay(size_t index) {
	primary_display_index = index;
}

void RenderSystem::AddRenderWindow(RenderWindow* Window) {
	render_windows.push_back(Window);
}

RenderWindow& RenderSystem::GetPrimaryRenderWindow() const {
	return *render_windows[0];
}

const std::vector<RenderWindow*>& RenderSystem::GetRenderWindows() const {
	return render_windows;
}

void RenderSystem::render_thread_func() {
	bgfx::init();
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
		0x303030ff, 1.0f, 0);

	bgfx::setDebug(BGFX_DEBUG_TEXT);

	InitializeVertexDeclarations();

	while (!Core::Engine::Get().IsStopped()) {
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