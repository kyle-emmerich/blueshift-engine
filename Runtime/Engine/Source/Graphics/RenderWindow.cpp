#include "RenderWindow.h"
#include "Core/Engine.h"
#include "bgfx/bgfx.h"

using namespace Blueshift;
using namespace Blueshift::Core;
using namespace Blueshift::Graphics;

RenderWindow::RenderWindow(uint32_t Width, uint32_t Height)
	: Window(Width, Height, true), view_width(Width), view_height(Height)
{ 
	if (Engine::Get().GetRenderSystem().GetRenderWindows().size() == 0) {
		is_primary = true;
		this->setup_platform();
	}
}

RenderWindow::~RenderWindow() {
	
}

void RenderWindow::resize_viewport(uint32_t width, uint32_t height) {
	view_width = width;
	view_height = height;
	dirty_viewport = true;
}

void RenderWindow::PreRender() {
	if (dirty_viewport) {
		dirty_viewport = false;
		if (is_primary) {
			bgfx::reset(view_width, view_height, BGFX_RESET_VSYNC);
		} else {
			//update framebuffer
		}
	}
	bgfx::setViewRect(0, 0, 0, view_width, view_height);
	bgfx::touch(0);
	bgfx::dbgTextClear();
	bgfx::dbgTextPrintf(0, 0, 0x4f, "test");
}

void RenderWindow::PostRender() {

}

 