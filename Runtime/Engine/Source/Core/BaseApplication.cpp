#include "Core/BaseApplication.h"
#include "Core/Engine.h"
#include "Graphics/RenderSystem.h"
#include "Storage/FileSystem.h"
#include <chrono>
#include <thread>

using namespace Blueshift;
using namespace Core;

BaseApplication::BaseApplication() { }

BaseApplication::~BaseApplication() {
	delete AppConfig;
	delete UserConfig;
}

void BaseApplication::Start(int argc, char* argv[]) {
	Engine = new Core::Engine(GetEngineSetup(), argc, argv);
	Engine->Timer.Start();
	Engine->CreateSystem<Storage::FileSystem>();
	AppConfig = new Utility::ConfigFile("app.cfg");
	UserConfig = new Utility::ConfigFile("user.cfg");

	Initialize();

	double frame_start = Engine->Timer.GetElapsedSeconds();
	double delta = 1.0 / 60.0;
	std::chrono::high_resolution_clock::time_point tp = std::chrono::high_resolution_clock::now();
	int desired_framerate = 60;
	std::chrono::high_resolution_clock::duration frame_time = std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(std::chrono::duration<double, std::ratio<1, 60>>(1));
	while (running) {
		frame_start = Engine->Timer.GetElapsedSeconds();
		running = Update(delta);

		delta = Engine->Timer.GetElapsedSeconds() - frame_start;
		if (delta < 1.0 / static_cast<double>(desired_framerate)) { //this needs to get changed.
			tp = std::chrono::high_resolution_clock::now() + frame_time;
			std::this_thread::sleep_until(tp);
		}
	}
	if (Engine->HasSystem<Graphics::RenderSystem>()) {
		Engine->GetSystem<Graphics::RenderSystem>()->WaitRenderThread();
	}
	Shutdown();
}

EngineParameters* BaseApplication::GetEngineSetup() {
	EngineParameters* Parameters = new EngineParameters;

	Parameters->Type = ApplicationType::Undefined;
	Parameters->Application = this;

	return Parameters;
}