#include "Core/BaseApplication.h"
#include "Core/Engine.h"
#include "Graphics/RenderSystem.h"
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
	Timer.Start();
	AppConfig = new Utility::ConfigFile("app.cfg");
	UserConfig = new Utility::ConfigFile("user.cfg");

	Initialize();

	double frame_start = Timer.GetElapsedSeconds();
	double delta = 1.0 / 60.0;
	std::chrono::high_resolution_clock::time_point tp = std::chrono::high_resolution_clock::now();
	int desired_framerate = DesiredFramerate;
	std::chrono::high_resolution_clock::duration frame_time = std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(std::chrono::duration<double, std::ratio<1, 60>>(1));
	while (running) {
		frame_start = Timer.GetElapsedSeconds();
		running = Update(delta);

		delta = Timer.GetElapsedSeconds() - frame_start;
		if (delta < 1.0 / static_cast<double>(DesiredFramerate)) {
			tp = std::chrono::high_resolution_clock::now() + frame_time;
			std::this_thread::sleep_until(tp);
		}
	}
	if (Engine->GetParameters()->Subsystems.Rendering) {
		Engine->GetRenderSystem().WaitRenderThread();
	}
	Shutdown();
}

EngineParameters* BaseApplication::GetEngineSetup() {
	EngineParameters* Parameters = new EngineParameters;

	Parameters->Type = ApplicationType::Undefined;
	Parameters->Application = this;

	Parameters->Subsystems.Rendering = false;
	Parameters->Subsystems.Input = false;
	Parameters->Subsystems.Audio = false;
	Parameters->Subsystems.Physics = false;
	Parameters->Subsystems.Networking = false;
	Parameters->Subsystems.AI = false;
	Parameters->Subsystems.API = false;
	Parameters->Subsystems.Storage = true;
	Parameters->Subsystems.Database = false;

	return Parameters;
}