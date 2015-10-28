#include "Graphics/RenderSystem.h"
#include "Graphics/VertexDeclarations.h"
#include "Core/Engine.h"

#include "Graphics/Model/Loader/OBJ.h"
#include <fstream>

using namespace Blueshift;
using namespace Blueshift::Graphics;
using namespace Blueshift::Platform;

using namespace Blueshift::Core;
using namespace Blueshift::Core::Math;

RenderSystem::RenderSystem() 
	: render_thread(&RenderSystem::render_thread_func, this) {
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

void RenderSystem::render_thread_func() {
	while (render_windows.size() == 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	bgfx::init();
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
		0x303030ff, 1.0f, 0);
	

	bgfx::setDebug(BGFX_DEBUG_STATS);

	InitializeVertexDeclarations();

	uint8_t* vs_code, * fs_code;
	std::ifstream reader("../Assets/Shaders/debug_vs.win.bin", std::ios::binary);
	reader.seekg(0, std::ios::end);
	size_t vs_size = reader.tellg();
	reader.seekg(0, std::ios::beg);
	vs_code = new uint8_t[vs_size];
	reader.read((char*)vs_code, vs_size);
	reader.close();
	reader.open("../Assets/Shaders/debug_fs.win.bin", std::ios::binary);
	reader.seekg(0, std::ios::end);
	size_t fs_size = reader.tellg();
	reader.seekg(0, std::ios::beg);
	fs_code = new uint8_t[fs_size];
	reader.read((char*)fs_code, fs_size);
	reader.close();

	reader.open("../Assets/ball.obj");

	Model::Loader::OBJ obj_loader;
	std::vector<std::unique_ptr<Model::OBJMeshData>> meshes;
	obj_loader.Load(reader, meshes);

	Shader* vs = new Shader(vs_code, vs_size);
	Shader* fs = new Shader(fs_code, fs_size);
	ShaderProgram program(vs, fs);

	fs->AddUniform("u_lightVec", Shader::UniformType::Vector, 1);


	float theta = 0.0f;

	Core::IApplication* application = Core::Engine::Get().GetParameters()->Application;
	while (application->IsRunning()) {
		for (RenderWindow* window : render_windows) {
			current_view_id = window->GetViewID();
			window->PreRender();
			
			theta += 0.01f;
			program["u_lightVec"] = Math::Vector3f(cos(theta), 0.0f, sin(theta));

			Math::Matrix4f model = Math::TranslationMatrix<float>(Vector3f(0.0f, 0.0f, 0.0f));
			Math::Matrix4f view = Inverse(Math::TranslationMatrix(Math::Vector3f(0.0f, 0.0f, -5.0f)));
			Math::Matrix4f proj = Math::Downgrade(Scene::CameraComponent::CreatePerspectiveTransform(window->GetAspectRatio(), 70.0, 0.1, 100.0));

			bgfx::setTransform(model.data);
			bgfx::setViewTransform(0, view.data, proj.data);
			
			meshes[0]->Render(&program);

			window->PostRender();
		}

		bgfx::frame();
	}

	bgfx::shutdown();
}