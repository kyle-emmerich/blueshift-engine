#include "Graphics/StaticMeshComponent.h"

using namespace Blueshift;
using namespace Graphics;

void StaticMeshComponent::Render(RenderSystem* system) {
	bgfx::setTransform(GetWorldTransform().data);
	if (albedo != nullptr) {
		(*program)["albedo"] = albedo;
	}
	if (material != nullptr) {
		(*program)["material"] = material;
	}
	if (normal != nullptr) {
		(*program)["normal"] = normal;
	}
	mesh->MeshData->Render(system, program, mesh);
}