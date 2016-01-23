#include "Core/Engine.h"
#include "Application/GameServer.h"

using namespace Blueshift;
using namespace Application;

EngineParameters* GameServer::GetEngineSetup() {
	EngineParameters* Parameters = BaseApplication::GetEngineSetup();

	Parameters->Type = ApplicationType::Server;

	return Parameters;
}