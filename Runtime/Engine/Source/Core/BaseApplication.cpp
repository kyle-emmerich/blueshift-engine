#include "Core/BaseApplication.h"
#include "Core/Engine.h"

using namespace Blueshift;
using namespace Core;

BaseApplication::BaseApplication() { }

BaseApplication::~BaseApplication() {
	Engine->Destroy();
}

void BaseApplication::Start(std::string AppCfgPath, std::string UserCfgPath) {
	Engine = new Core::Engine(GetEngineSetup());
	Timer.Start();
	AppConfig.ReadFile(AppCfgPath);
	UserConfig.ReadFile(UserCfgPath);

	Initialize();

	double frame_start = Timer.GetElapsedSeconds();
	double delta = 1.0 / 60.0;
	while (running) {
		frame_start = Timer.GetElapsedSeconds();
		running = Update(delta);

		delta = Timer.GetElapsedSeconds() - frame_start;
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
	Parameters->Subsystems.Storage = false;
	Parameters->Subsystems.Database = false;

	return Parameters;
}