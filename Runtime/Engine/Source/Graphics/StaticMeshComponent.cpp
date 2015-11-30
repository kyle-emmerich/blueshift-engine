#include "Graphics/StaticMeshComponent.h"

using namespace Blueshift;
using namespace Graphics;

void StaticMeshComponent::Render(RenderSystem* system) {
	bgfx::setState(
		BGFX_STATE_MSAA |
		BGFX_STATE_ALPHA_WRITE |
		BGFX_STATE_RGB_WRITE |
		BGFX_STATE_CULL_CCW |
		BGFX_STATE_DEPTH_WRITE |
		BGFX_STATE_DEPTH_TEST_LEQUAL
	);
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
	(*program)["u_lightPos"] = Core::Math::Vector4(0.0f, 10.0f, 10.0f);
	mesh->MeshData->Render(system, program, mesh);
}