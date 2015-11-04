#include "Core/GameServer.h"
#include "Core/Engine.h"

using namespace Blueshift;
using namespace Core;

EngineParameters* GameServer::GetEngineSetup() {
	EngineParameters* Parameters = BaseApplication::GetEngineSetup();

	Parameters->Type = ApplicationType::Server;

	return Parameters;
}