#include "Core/BaseApplication.h"
#include "Core/Engine.h"
#include "Scene/SceneGraph.h"
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
	delete graph;
}

void BaseApplication::RegisterFileTypes(Storage::AssetSystem* Assets) {
	Assets->RegisterFileType<Graphics::Model::IQM::Loader>("iqm");
	
}

void BaseApplication::Start(int argc, char* argv[]) {
	Engine = new Core::Engine(GetEngineSetup(), argc, argv);
	Engine->Timer.Start();
	Engine->CreateSystem<Storage::FileSystem>();
	Engine->CreateSystem<Storage::AssetSystem>();
	RegisterFileTypes(Engine->GetSystem<Storage::AssetSystem>());

	AppConfig = new Utility::ConfigFile("app.cfg");
	UserConfig = new Utility::ConfigFile("user.cfg");

	graph = new Scene::SceneGraph(Engine);

	Initialize();

	

	double frame_start = Engine->Timer.GetElapsedSeconds();
	double delta = 1.0 / 60.0;
	double desired_delta = delta;
	std::chrono::high_resolution_clock::time_point tp = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::duration frame_time = std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(std::chrono::duration<double, std::ratio<1, 1000000>>(1));
	while (running) {
		std::unique_lock<std::mutex> lock(FrameMutex);
		IsFrameReady = false;
		frame_start = Engine->Timer.GetElapsedSeconds();
		running = Update(delta); 

		//this whole section sucks, rip out and redo some time

		delta = Engine->Timer.GetElapsedSeconds() - frame_start;
		lock.unlock();
		IsFrameReady = true;
		FrameReady.notify_one();

		while (desired_delta - delta > 1e-5) {
			std::this_thread::sleep_for(frame_time);
			delta = Engine->Timer.GetElapsedSeconds() - frame_start;
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