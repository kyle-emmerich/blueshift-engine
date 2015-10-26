#include "Graphics/RenderSystem.h"
#include "Graphics/VertexDeclarations.h"
#include "Core/Engine.h"

#include <fstream>

using namespace Blueshift;
using namespace Blueshift::Graphics;
using namespace Blueshift::Platform;

using namespace Blueshift::Core;
using namespace Blueshift::Core::Math;

static SimpleVertex vertices[24] = {
	//+Y Up
	SimpleVertex(Vector3f(-1.0f, 1.0f, -1.0f),
				 Vector3f(0.0f, 1.0f, 0.0f)),
	SimpleVertex(Vector3f( 1.0f, 1.0f, -1.0f),
				 Vector3f(0.0f, 1.0f, 0.0f)),
	SimpleVertex(Vector3f(-1.0f, 1.0f, 1.0f),
				 Vector3f(0.0f, 1.0f, 0.0f)),
	SimpleVertex(Vector3f(1.0f, 1.0f, 1.0f),
				 Vector3f(0.0f, 1.0f, 0.0f)),

	//-Y Down
	SimpleVertex(Vector3f(1.0f, -1.0f, 1.0f),
				 Vector3f(0.0f, -1.0f, 0.0f)),
	SimpleVertex(Vector3f(-1.0f, -1.0f, 1.0f),
				 Vector3f(0.0f, -1.0f, 0.0f)),
	SimpleVertex(Vector3f(1.0f, -1.0f, -1.0f),
				 Vector3f(0.0f, -1.0f, 0.0f)),
	SimpleVertex(Vector3f(-1.0f, -1.0f, -1.0f),
				 Vector3f(0.0f, -1.0f, 0.0f)),
	
	//+Z Front
	SimpleVertex(Vector3f(-1.0f, -1.0f, 1.0f),
				 Vector3f(0.0f, 0.0f, 1.0f)),
	SimpleVertex(Vector3f(1.0f, -1.0f, 1.0f),
				 Vector3f(0.0f, 0.0f, 1.0f)),
	SimpleVertex(Vector3f(-1.0f, 1.0f, 1.0f),
				 Vector3f(0.0f, 0.0f, 1.0f)),
	SimpleVertex(Vector3f(1.0f, 1.0f, 1.0f),
				 Vector3f(0.0f, 0.0f, 1.0f)),

	//-Z Back
	SimpleVertex(Vector3f(-1.0f, -1.0f, -1.0f),
				 Vector3f(0.0f, 0.0f, -1.0f)),
	SimpleVertex(Vector3f(1.0f, -1.0f, -1.0f),
				 Vector3f(0.0f, 0.0f, -1.0f)),
	SimpleVertex(Vector3f(-1.0f, 1.0f, -1.0f),
				 Vector3f(0.0f, 0.0f, -1.0f)),
	SimpleVertex(Vector3f(1.0f, 1.0f, -1.0f),
				 Vector3f(0.0f, 0.0f, -1.0f)),

	//+X Right
	SimpleVertex(Vector3f(1.0f, -1.0f, -1.0f),
				 Vector3f(1.0f, 0.0f, 0.0f)),
	SimpleVertex(Vector3f(1.0f, -1.0f, 1.0f),
				 Vector3f(1.0f, 0.0f, 0.0f)),
	SimpleVertex(Vector3f(1.0f, 1.0f, -1.0f),
				 Vector3f(1.0f, 0.0f, 0.0f)),
	SimpleVertex(Vector3f(1.0f, 1.0f, 1.0f),
				 Vector3f(1.0f, 0.0f, 0.0f)),

	//-X Left
	SimpleVertex(Vector3f(-1.0f, -1.0f, -1.0f),
				 Vector3f(-1.0f, 0.0f, 0.0f)),
	SimpleVertex(Vector3f(-1.0f, -1.0f, 1.0f),
				 Vector3f(-1.0f, 0.0f, 0.0f)),
	SimpleVertex(Vector3f(-1.0f, 1.0f, -1.0f),
				 Vector3f(-1.0f, 0.0f, 0.0f)),
	SimpleVertex(Vector3f(-1.0f, 1.0f, 1.0f),
				 Vector3f(-1.0f, 0.0f, 0.0f)),
};
static const uint16_t indices[36] = {
	0, 1, 2,
	2, 1, 3,

	6, 5, 4,
	7, 5, 6,
	//4, 5, 6,
	//6, 5, 7,

	10, 9, 8,
	11, 9, 10,
	//8, 9, 10,
	//10, 9, 11,

	12, 13, 14,
	14, 13, 15,

	16, 17, 18,
	18, 17, 19,

	22, 21, 20,
	23, 21, 22
	//20, 21, 22,
	//22, 21, 23
};

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
	

	//bgfx::setDebug(BGFX_DEBUG_TEXT);

	InitializeVertexDeclarations();

	//debug stuff
	bgfx::VertexBufferHandle evil_vbh;
	bgfx::IndexBufferHandle evil_ibh;
	bgfx::ProgramHandle evil_prog;

	size_t size = sizeof(vertices);
	const bgfx::Memory* mem = bgfx::makeRef(vertices, static_cast<uint32_t>(size));
	evil_vbh = bgfx::createVertexBuffer(mem, SimpleVertex::decl);
	evil_ibh = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));

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

	bgfx::ShaderHandle vs = bgfx::createShader(bgfx::makeRef(vs_code, vs_size));
	bgfx::ShaderHandle fs = bgfx::createShader(bgfx::makeRef(fs_code, fs_size));

	evil_prog = bgfx::createProgram(vs, fs, true);

	//delete[] vs_code;
	//delete[] fs_code;

	float theta = 0.0f;

	Core::IApplication* application = Core::Engine::Get().GetParameters()->Application;
	while (application->IsRunning()) {
		
		for (RenderWindow* window : render_windows) {
			current_view_id = window->GetViewID();
			window->PreRender();
			
			theta += 0.01f;
			Math::Matrix4f model = Math::RotationMatrix<4, float>(Math::Vector3f(1.0f, 1.0f, 0.0f), theta) * Math::TranslationMatrix<float>(Vector3f(0.0f, 0.0f, 5.0f));
			Math::Matrix4f view = Inverse(Math::TranslationMatrix(Math::Vector3f(0.0f, 0.0f, -10.0f)));
			Math::Matrix4f proj = Math::Downgrade(Scene::CameraComponent::CreatePerspectiveTransform(window->GetAspectRatio(), 70.0, 0.1, 100.0));

			bgfx::setTransform(model.data);
			bgfx::setViewTransform(0, view.data, proj.data);
			bgfx::setVertexBuffer(evil_vbh);
			bgfx::setIndexBuffer(evil_ibh);

			

			bgfx::submit(0, evil_prog);

			window->PostRender();
		}

		bgfx::frame();
	}

	bgfx::shutdown();
}