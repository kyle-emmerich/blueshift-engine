#include "Graphics/RenderWindow.h"
#include "Graphics/RenderSystem.h"
#include "Core/Engine.h"
#include "bgfx/bgfx.h"

using namespace Blueshift;
using namespace Blueshift::Core;
using namespace Blueshift::Graphics;



RenderWindow::RenderWindow(uint32_t Width, uint32_t Height, Scene::SceneGraph* scene)
	: Window(Width, Height, true), view_width(Width), view_height(Height), graph(scene) { 
	Graphics::RenderSystem* rendering = Engine::Get().GetSystem<RenderSystem>();
	
	this->view_id = static_cast<uint8_t>(rendering->GetRenderWindows().size());
	if (Engine::Get().GetSystem<RenderSystem>()->GetRenderWindows().size() == 0) {
		this->is_primary = true;
		this->setup_platform();
	}
}

RenderWindow::~RenderWindow() {
	
}

void RenderWindow::resize_viewport(uint32_t width, uint32_t height) {
	this->view_width = width;
	this->view_height = height;
	this->dirty_viewport = true;
}

void RenderWindow::PreRender() {
	if (this->dirty_viewport) {
		this->dirty_viewport = false;
		if (this->is_primary) {
			bgfx::setViewFrameBuffer(this->view_id, BGFX_INVALID_HANDLE);
			bgfx::reset(this->view_width, this->view_height,
				BGFX_RESET_VSYNC |
				BGFX_RESET_MSAA_X16 |
				BGFX_RESET_SRGB_BACKBUFFER
			);
		} else {
			//update framebuffer
		}
	}
	bgfx::setViewClear(this->view_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
	bgfx::setViewRect(this->view_id, 0, 0, this->view_width, this->view_height);
	
	CameraComponent* cam_component = (CameraComponent*)this->camera;
	bgfx::setViewTransform(this->view_id, cam_component->GetViewTransform().data, cam_component->GetProjection().data);
	
	bgfx::touch(this->view_id);

}

void RenderWindow::PostRender() {

}