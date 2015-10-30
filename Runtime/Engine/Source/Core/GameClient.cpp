#include "Core/GameClient.h"
#include "Core/Engine.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/RenderWindow.h"

using namespace Blueshift;
using namespace Core;
using namespace Graphics;

EngineParameters* GameClient::GetEngineSetup() {
	EngineParameters* Parameters = BaseApplication::GetEngineSetup();

	Parameters->Type = ApplicationType::Client;

	Parameters->Subsystems.Rendering = true;
	Parameters->Subsystems.Input = true;
	Parameters->Subsystems.Audio = true;
	Parameters->Subsystems.Physics = true;
	Parameters->Subsystems.Networking = true;
	Parameters->Subsystems.AI = false;
	Parameters->Subsystems.API = true;
	Parameters->Subsystems.Storage = true;
	Parameters->Subsystems.Database = false;

	return Parameters;
}

void GameClient::Initialize() {
	Engine->GetRenderSystem().SetPrimaryDisplay(AppConfig->Get<size_t>("Window", "DisplayID"));
	main_window = new RenderWindow(
		AppConfig->Get<uint32_t>("Window", "Width", 1280),
		AppConfig->Get<uint32_t>("Window", "Height", 720)
	);
	main_window->SetTitle(Engine->GetParameters()->ApplicationName);

	if (AppConfig->Get<bool>("Window", "IsFullscreen", false)) {
		main_window->SetFullscreen(true);
	}
	if (AppConfig->Get<bool>("Window", "IsFullscreenDesktop", false)) {
		main_window->SetFullscreenDesktop(true);
	}
	
	Engine->GetRenderSystem().AddRenderWindow(main_window);
}

void GameClient::Shutdown() {
	main_window->Close();
}

bool GameClient::Update(double dt) {
	return main_window->ProcessEvents();
}