#include "Core/GameClient.h"
#include "Core/Engine.h"
#include "Graphics/RenderWindow.h"
#include "Input/InputSystem.h"
#include "Storage/FileSystem.h"
#include "Graphics/RenderSystem.h"

using namespace Blueshift;
using namespace Application;
using namespace Graphics;

Core::EngineParameters* GameClient::GetEngineSetup() {
	Core::EngineParameters* Parameters = BaseApplication::GetEngineSetup();

	Parameters->Type = ApplicationType::Client;

	return Parameters;
}

void GameClient::Initialize() {
	this->Engine->CreateSystem<Graphics::RenderSystem>();
	this->Engine->CreateSystem<Input::InputSystem>();
	this->Engine->SetLogFile(new Storage::File("log.txt", Storage::FileOpenMode::Append));

	this->Engine->GetSystem<Graphics::RenderSystem>()->SetPrimaryDisplay(AppConfig->Get<size_t>("Window", "DisplayID", 0));
	this->main_window = new RenderWindow(
		this->AppConfig->Get<uint32_t>("Window", "Width", 1280),
		this->AppConfig->Get<uint32_t>("Window", "Height", 720)
	);
	this->main_window->SetTitle(Engine->GetParameters()->ApplicationName);

	if (this->AppConfig->Get<bool>("Window", "IsFullscreen", false)) {
		this->main_window->SetFullscreen(true);
	}
	if (this->AppConfig->Get<bool>("Window", "IsFullscreenDesktop", false)) {
		this->main_window->SetFullscreenDesktop(true, AppConfig->Get<bool>("Window", "SpanAllDisplays", false));
	}
	
	this->Engine->GetSystem<Graphics::RenderSystem>()->AddRenderWindow(this->main_window);
}

void GameClient::Shutdown() {
	this->main_window->Close();
}

bool GameClient::Update(double dt) {
	//This is where systems get invoked.
	//But there aren't any ready yet, so just keep that in mind.

	return this->main_window->ProcessEvents();
}