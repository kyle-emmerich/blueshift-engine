#include "Core/GameClient.h"
#include <iostream>

class TestClient : public Blueshift::Core::GameClient {
public:
	inline TestClient() : Blueshift::Core::GameClient() {}

	virtual Blueshift::Core::EngineParameters* GetEngineSetup();

	virtual void Initialize();
	virtual void Shutdown();

	virtual bool Update(double dt);
};

Blueshift::Core::EngineParameters* TestClient::GetEngineSetup() {
	Blueshift::Core::EngineParameters* Parameters = GameClient::GetEngineSetup();

	Parameters->ApplicationName = "Blueshift Client Test";

	return Parameters;
}

void TestClient::Initialize() {
	//One little thing: change the default display to (my) left one
	Engine->GetRenderSystem().SetPrimaryDisplay(1);

	GameClient::Initialize();
}

void TestClient::Shutdown() {
	GameClient::Shutdown();
}

bool TestClient::Update(double dt) {
	
	return GameClient::Update(dt);
}

int main(int argc, char* argv[]) {
	TestClient().Start();
	return 0;
} 