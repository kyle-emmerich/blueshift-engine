#include "Core/GameClient.h"
#include "Core/Engine.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/RenderWindow.h"
#include "Input/InputSystem.h"
#include "Storage/FileSystem.h"

using namespace Blueshift;
using namespace Core;
using namespace Graphics;

EngineParameters* GameClient::GetEngineSetup() {
	EngineParameters* Parameters = BaseApplication::GetEngineSetup();

	Parameters->Type = ApplicationType::Client;

	return Parameters;
}

void GameClient::Initialize() {
	Engine->CreateSystem<Graphics::RenderSystem>();
	Engine->CreateSystem<Input::InputSystem>();
	Engine->SetLogFile(new Storage::File("log.txt", Storage::FileOpenMode::Append));

	Engine->GetSystem<Graphics::RenderSystem>()->SetPrimaryDisplay(AppConfig->Get<size_t>("Window", "DisplayID", 0));
	main_window = new RenderWindow(
		AppConfig->Get<uint32_t>("Window", "Width", 1280),
		AppConfig->Get<uint32_t>("Window", "Height", 720),
		graph
	);
	main_window->SetTitle(Engine->GetParameters()->ApplicationName);

	if (AppConfig->Get<bool>("Window", "IsFullscreen", false)) {
		main_window->SetFullscreen(true);
	}
	if (AppConfig->Get<bool>("Window", "IsFullscreenDesktop", false)) {
		main_window->SetFullscreenDesktop(true, AppConfig->Get<bool>("Window", "SpanAllDisplays", false));
	}
	
	Engine->GetSystem<Graphics::RenderSystem>()->AddRenderWindow(main_window);
}

void GameClient::Shutdown() {
	main_window->Close();
}

bool GameClient::Update(double dt) {
	//This is where systems get invoked.
	//But there aren't any ready yet, so just keep that in mind.

	return main_window->ProcessEvents();
}