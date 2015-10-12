#include "Core/GameServer.h"
#include "Core/Engine.h"

using namespace Blueshift;
using namespace Core;

EngineParameters* GameServer::GetEngineSetup() {
	EngineParameters* Parameters = BaseApplication::GetEngineSetup();

	Parameters->Type = ApplicationType::Server;

	Parameters->Subsystems.Rendering = false;
	Parameters->Subsystems.Input = false;
	Parameters->Subsystems.Audio = false;
	Parameters->Subsystems.Physics = true;
	Parameters->Subsystems.Networking = true;
	Parameters->Subsystems.AI = true;
	Parameters->Subsystems.API = true;
	Parameters->Subsystems.Storage = true;
	Parameters->Subsystems.Database = true;

	return Parameters;
}