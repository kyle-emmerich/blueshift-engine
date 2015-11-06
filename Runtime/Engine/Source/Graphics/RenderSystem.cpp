#include "Graphics/RenderSystem.h"
#include "Graphics/VertexDeclarations.h"
#include "Graphics/RenderableComponent.h"
#include "Core/Engine.h"


#include "Graphics/Model/Loader/OBJ.h"
#include "Graphics/Texture.h"
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

void RenderSystem::ProcessComponents(std::vector<Scene::Component>& components) {
	RenderableComponent* renderables = dynamic_cast<RenderableComponent*>(components.data());
	size_t size = components.size();
	for (size_t i = 0; i < size; i++) {
		RenderableComponent* component = renderables + i;
		component->Render(this);
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
		bgfx::setDebug(BGFX_DEBUG_STATS);

		//Create a fullscreen quad
		Vector2f fs_verts[4] = {
			Vector2f(-1.0f, -1.0f),
			Vector2f(1.0f, -1.0f),
			Vector2f(-1.0f,  1.0f),
			Vector2f(1.0f,  1.0f)
		};
		uint16_t fs_indices[6] = {
			0, 1, 2, 2, 1, 3
		};
		VertexType fs_type;
		fs_type.begin().add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float).end();
		Model::MeshBounds bounds;
		Model::StaticMeshData<Vector2f, uint16_t> fs_quad("FullscreenQuad", fs_verts, 4, fs_type, fs_indices, 6, bounds);

		Shader* fs_vert = new Shader("Shaders/background.vert.win.bin");
		fs_vert->Complete();
		Shader* fs_frag = new Shader("Shaders/background.frag.win.bin");
		fs_frag
			->AddUniform("u_cubemap", Shader::UniformType::Texture)
			->AddUniform("u_view", Shader::UniformType::Matrix4)
			->Complete();
		ShaderProgram fs_program(fs_vert, fs_frag);

		Model::Loader::OBJ obj_loader;
		std::vector<std::unique_ptr<Model::OBJMeshData>> meshes;
		obj_loader.Load("ball.obj", meshes);

		Shader* vs = new Shader("Shaders/debug_vs.win.bin");
		vs->Complete();

		Shader* fs = new Shader("Shaders/debug_fs.win.bin");
		fs->AddUniform("u_samples", Shader::UniformType::Vector, 1)
			->AddUniform("u_lightVec", Shader::UniformType::Vector, 1)
			->AddUniform("u_texture", Shader::UniformType::Texture, 1)
			->Complete();

		ShaderProgram program(vs, fs);
		Texture tex("space.dds");

		float theta = 0.0f;

		float samples = 20;

		Core::IApplication* application = Core::Engine::Get().GetParameters()->Application;
		while (application->IsRunning()) {
			for (RenderWindow* window : render_windows) {
				current_view_id = window->GetViewID();
				window->PreRender();

				Math::Matrix4f view = Inverse(Math::TranslationMatrix<float>(Vector3f(0.0f, 2.0f, -20.0f)) /** Math::RotationMatrix<4, float>(Vector3f(0.0f, 1.0f, 0.0f), theta)*/);
				Math::Matrix4f proj = Math::Downgrade(Scene::CameraComponent::CreatePerspectiveTransform(window->GetAspectRatio(), 60.0, 0.1, 100.0));
				bgfx::setViewTransform(0, view.data, proj.data);

				//draw the background
				tex.AssignTo(0, fs_program["u_cubemap"]);
				fs_quad.Render(this, &fs_program);

				//render components


				theta += 0.01f;


				
				int n = 10;
				for (int j = -n/2; j < n/2; j++) {
					for (int i = -n/2; i < n/2; i++) {
						Math::Matrix4f model =
							Math::RotationMatrix<4, float>(Math::Vector3f(1.0, 0.0, 0.0), theta + Math::ToRadians(180.0f)) *
							Math::TranslationMatrix<float>(Vector3f(i * 2.0f, i%2 * j%2, j * 2.0f));
						program["u_samples"].Set(&samples);
						program["u_lightVec"] = Math::Vector3f(0.25f, -1.0f, 0.25f);

						tex.AssignTo(0, program["u_texture"]);

						bgfx::setTransform(model.data);
						bgfx::setState(
							BGFX_STATE_MSAA |
							BGFX_STATE_ALPHA_WRITE |
							BGFX_STATE_RGB_WRITE |
							BGFX_STATE_CULL_CCW |
							BGFX_STATE_DEPTH_WRITE |
							BGFX_STATE_DEPTH_TEST_LEQUAL
							);

						meshes[0]->Render(this, &program);
					}
				}

				window->PostRender();
			}

			bgfx::frame();
		}
	}
	bgfx::shutdown();
}