#include "Core/GameClient.h"
#include "Graphics/Model/Loader/OBJ.h"
#include "Graphics/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include "Physics/RigidbodyComponent.h"
#include "Scripting/LuaState.h"
#include "Input/InputSystem.h"
#include <iostream>

using namespace Blueshift;
using namespace Core::Math;

class TestClient : public Core::GameClient {
protected:
	float cam_speed = 30.0f;
	float look_sensitivity = 0.1f;
	Vector4 camera_pos = Vector4(0.0f, 0.0f, 10.0f, 1.0f);
	Quaternion camera_rot;
	float yaw = 3.14159f; float pitch = 0.0f;

	Scripting::LuaState* lua;

	Scene::Object* model;
	Graphics::Model::OBJMeshData* mesh;
	Graphics::ShaderProgram* prog;

	std::vector<std::unique_ptr<Graphics::Model::OBJMeshData>>* meshes;

public:
	inline TestClient() : Blueshift::Core::GameClient() {}

	virtual Core::EngineParameters* GetEngineSetup();

	virtual void Initialize();
	virtual void InitializeRenderData();
	virtual void DestroyRenderData();
	virtual void Shutdown();

	virtual bool Update(double dt);
};

Core::EngineParameters* TestClient::GetEngineSetup() {
	Core::EngineParameters* Parameters = GameClient::GetEngineSetup();

	Parameters->ApplicationName = "Blueshift Client Test";
	Parameters->ApplicationIdentity = "Blueshift";

	return Parameters;
}

void TestClient::Initialize() {
	GameClient::Initialize();

	Scene::Object* obj = new Scene::Object;
	Scene::Component::Handle handle = graph->AllocateComponent<Graphics::CameraComponent>(obj);
	Graphics::CameraComponent* camera = handle;
	camera->SetProjection(camera->CreatePerspectiveTransform(main_window->GetAspectRatio()));
	camera->UpdateWorldTransform();
	main_window->SetCamera(handle);



	model = new Scene::Object;
	//	Scene::Component::Handle phys_handle = graph->AllocateComponent<Physics::RigidbodyComponent>(model);

	Engine->Console.SetVisible(false); 


	lua = new Scripting::LuaState;
	Engine->Console.SetLuaState(lua);
	try {
		lua->ExecuteFile("test.lua");
	} catch (LuaError& e) {
		Engine->Log(e);
	}

}

void TestClient::InitializeRenderData() {
	Graphics::Model::Loader::OBJ loader;
	
	meshes = new std::vector<std::unique_ptr<Graphics::Model::OBJMeshData>>;
	loader.Load("cube.obj", *meshes);
	mesh = &(*(*meshes)[0]);
	Scene::Component::Handle mesh_handle = graph->AllocateComponent<Graphics::StaticMeshComponent>(model);
	Graphics::StaticMeshComponent* mesh_comp = mesh_handle;
	mesh_comp->SetMesh(&mesh->GetSection(0));

	Graphics::Shader* vs = new Graphics::Shader("Shaders/debug_vs.win.bin");
	vs->Complete();
	Graphics::Shader* fs = new Graphics::Shader("Shaders/debug_fs.win.bin");
	fs->AddUniform("u_lightPos", Graphics::Shader::UniformType::Vector);
	//fs->AddUniform("u_texture", Graphics::Shader::UniformType::Texture);
	fs->Complete();
	prog = new Graphics::ShaderProgram(vs, fs);
	mesh_comp->SetProgram(prog);
}

void TestClient::DestroyRenderData() {
	delete prog;
	delete meshes;
}

void TestClient::Shutdown() {
	GameClient::Shutdown();
}

bool TestClient::Update(double dt) {
	Graphics::CameraComponent* camera = main_window->GetCamera();
	Engine->GetSystem<Input::InputSystem>()->PollDevices();

	Vector4 camera_dv(0.0f, 0.0f, 0.0f, 0.0f);
	if (!Engine->Console.IsVisible()) {
		//let's do basic camera controls
		//TODO: write better input API because this SUCKS
		//TODO: move this to a component
		


	}
	
	

	//now rotate the camera_dv vector by the rotation matrix
	camera_rot = QuaternionFromAxisAngle(Vector4(1.0f, 0.0f, 0.0f), pitch) * QuaternionFromAxisAngle(Vector4(0.0f, 1.0f, 0.0f), yaw);
	Normalize(camera_rot);
	Matrix4 camera_rot_mat = QuaternionToMatrix4(camera_rot);
	camera_dv = camera_rot_mat * camera_dv * cam_speed;
	//Now we can translate the position (point) by the dv (vector)
	camera_pos += camera_dv * static_cast<float>(dt);

	camera->SetOrientation(camera_rot);
	camera->SetPosition(camera_pos);
	camera->UpdateWorldTransform();

	return GameClient::Update(dt);
}

int main(int argc, char* argv[]) {
	TestClient().Start(argc, argv);
	return 0;
} 