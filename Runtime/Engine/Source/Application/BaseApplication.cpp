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
	this->Engine = new Core::Engine(this->GetEngineSetup(), argc, argv);
	this->Engine->Timer.Start();
	this->Engine->CreateSystem<Storage::FileSystem>();
	this->Engine->CreateSystem<Storage::AssetSystem>();
	RegisterFileTypes(this->Engine->GetSystem<Storage::AssetSystem>());

	AppConfig = new Utility::ConfigFile("app.cfg");
	UserConfig = new Utility::ConfigFile("user.cfg");
	
	Initialize();
	MainLoop();
	Shutdown();
}

EngineParameters* BaseApplication::GetEngineSetup() {
	EngineParameters* Parameters = new EngineParameters;

	Parameters->Type = ApplicationType::Undefined;
	Parameters->Application = this;

	return Parameters;
}