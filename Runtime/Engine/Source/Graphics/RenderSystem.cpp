#include "Graphics/RenderSystem.h"
#include "Graphics/VertexDeclarations.h"
#include "Graphics/RenderableComponent.h"
#include "Core/Engine.h"
#include "bgfx/bgfx.h"

#include "Graphics/StaticMeshComponent.h"
#include "Graphics/Model/Loader/OBJ.h"
#include "Graphics/Texture/Texture.h"
#include <fstream>

using namespace Blueshift;
using namespace Blueshift::Graphics;
using namespace Blueshift::Platform;

using namespace Blueshift::Core;
using namespace Blueshift::Core::Math;

RenderSystem::RenderSystem(Core::Engine* engine) 
	: render_thread(&RenderSystem::render_thread_func, this) {
	DisplayInfo::EnumerateDisplays(this); 
	primary_display_index = 0;
}

RenderSystem::~RenderSystem() {
	for (auto* window : render_windows) {
		if (window != nullptr) {
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

	if (primary_display_index >= displays.size()) {
		primary_display_index = 0;
	}
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

void RenderSystem::WaitRenderThread() {
	if (render_thread.joinable()) {
		render_thread.join();
	}
}

void RenderSystem::render_thread_func() {
	while (render_windows.size() == 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	bgfx::init();
	{
		InitializeVertexDeclarations();
		bgfx::setDebug(BGFX_DEBUG_TEXT);

		//Create a fullscreen quad
		Vector4 fs_verts[4] = {
			Vector4(-1.0f, -1.0f),
			Vector4(1.0f, -1.0f),
			Vector4(-1.0f,  1.0f),
			Vector4(1.0f,  1.0f)
		};
		uint16_t fs_indices[6] = {
			0, 1, 2, 2, 1, 3
		};
		VertexType fs_type;
		fs_type.begin().add(bgfx::Attrib::Position, 4, bgfx::AttribType::Float).end();
		Model::MeshBounds bounds;
		Model::StaticMeshData<Vector4, uint16_t> fs_quad(fs_verts, 4, fs_type, fs_indices, 6, bounds);

		Shader* fs_vert = new Shader("Shaders/background.vert.win.bin");
		fs_vert->Complete();
		Shader* fs_frag = new Shader("Shaders/background.frag.win.bin");
		fs_frag
			->AddUniform("u_cubemap", Shader::UniformType::Texture)
			->AddUniform("u_view", Shader::UniformType::Matrix4)
			->Complete();
		ShaderProgram fs_program(fs_vert, fs_frag);
		Texture::Texture tex("space.dds");

		Core::IApplication* application = Core::Engine::Get().GetParameters()->Application;
		application->InitializeRenderData();

		double start = Engine::Get().Timer.GetElapsedSeconds();
		while (application->IsRunning()) {
			std::unique_lock<std::mutex> lock(application->FrameMutex);
			while (!application->IsFrameReady) {
				application->FrameReady.wait(lock);
			}
			application->IsFrameReady = false;
			
			for (RenderWindow* window : render_windows) {
				this->current_view_id = window->GetViewID();
				Scene::SceneGraph* scene = window->GetSceneGraph();
				if (scene == nullptr) {
					continue;
				}

				window->PreRender();
				
				//draw the background
				//TODO: move this to... camera? scenegraph?
				tex.AssignTo(0, fs_program["u_cubemap"]);
				fs_quad.Render(this, &fs_program);

				//render components
				ProcessComponents<StaticMeshComponent>(scene);

				Engine::Get().Console.Render();

				window->PostRender();
			}

			bgfx::frame();
		}
		for (auto window : render_windows) {
			auto graph = window->GetSceneGraph();
			if (graph != nullptr) {
				graph->DestroyComponents<StaticMeshComponent>();
			}
		}
		application->DestroyRenderData();
	}
	bgfx::shutdown();
}