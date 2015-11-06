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
	Parameters->ApplicationIdentity = "Blueshift";

	return Parameters;
}

void TestClient::Initialize() {
	GameClient::Initialize();

	//One little thing: change the default display to (my) left one
	Engine->GetSystem<Blueshift::Graphics::RenderSystem>()->SetPrimaryDisplay(1);
}

void TestClient::Shutdown() {
	GameClient::Shutdown();
}

bool TestClient::Update(double dt) {
	
	return GameClient::Update(dt);
}

int main(int argc, char* argv[]) {
	TestClient().Start(argc, argv);
	return 0;
} 