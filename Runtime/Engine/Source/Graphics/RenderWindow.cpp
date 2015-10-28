#include "RenderWindow.h"
#include "Core/Engine.h"
#include "bgfx/bgfx.h"

using namespace Blueshift;
using namespace Blueshift::Core;
using namespace Blueshift::Graphics;



RenderWindow::RenderWindow(uint32_t Width, uint32_t Height)
	: Window(Width, Height, true), view_width(Width), view_height(Height)
{ 
	view_id = static_cast<uint8_t>(Engine::Get().GetRenderSystem().GetRenderWindows().size());
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
			bgfx::setViewFrameBuffer(view_id, BGFX_INVALID_HANDLE);
			bgfx::reset(view_width, view_height,
				BGFX_RESET_VSYNC |
				BGFX_RESET_MSAA_X8 |
				BGFX_RESET_SRGB_BACKBUFFER
				);

		} else {
			//update framebuffer
		}
	}
	bgfx::setViewRect(view_id, 0, 0, view_width, view_height);
	bgfx::touch(view_id);
	bgfx::setState(
		BGFX_STATE_MSAA |
		BGFX_STATE_ALPHA_WRITE |
		BGFX_STATE_RGB_WRITE |
		BGFX_STATE_CULL_CCW |
		BGFX_STATE_DEPTH_WRITE |
		BGFX_STATE_DEPTH_TEST_LESS);
}

void RenderWindow::PostRender() {

}

 