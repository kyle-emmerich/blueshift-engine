#include "Physics/Interop/DebugDraw.h"
#include "Physics/Interop/Math.h"
#include "Core/Engine.h"
#include "Graphics/RenderData.h"
#include "Graphics/RenderSystem.h"

using namespace Blueshift;
using namespace Physics;
using namespace Interop;
using namespace Core::Math;

DebugDraw::DebugDraw() {
	Graphics::RenderData vert_data = bgfx::makeRef(vb_vertices, sizeof(debug_vertex) * max_verts);
	bgfx::VertexDecl decl;
	decl.begin().add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float).add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8).end();
	vb = bgfx::createDynamicVertexBuffer(vert_data, decl);

	vs = new Graphics::Shader("Shaders/DebugLine.vert.bin");
	fs = new Graphics::Shader("Shaders/DebugLine.frag.bin");
	prog = new Graphics::ShaderProgram(vs, fs);
}

DebugDraw::~DebugDraw() {
	delete vs;
	delete fs;
	delete prog;

	bgfx::destroyDynamicVertexBuffer(vb);
}

void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
	if (pos >= max_verts) {
		dropped++;
		return;
	}

	ColorRGBA8 c = Core::Math::ColorRGBA8(
		static_cast<float>(color.m_floats[0]),
		static_cast<float>(color.m_floats[1]),
		static_cast<float>(color.m_floats[2]),
		1.0f
	);

	//2 new vertices
	vb_vertices[pos].position = FromBullet<float>(from);
	vb_vertices[pos].color = c;
	pos++;
	vb_vertices[pos].position = FromBullet<float>(to);
	vb_vertices[pos].color = c;
	pos++;
}

void DebugDraw::Render(Matrix4f view, Matrix4f projection) {
	bgfx::setState(
		BGFX_STATE_RGB_WRITE |
		BGFX_STATE_ALPHA_WRITE |
		BGFX_STATE_PT_LINES |
		BGFX_STATE_MSAA |
		BGFX_STATE_DEPTH_TEST_LEQUAL
	);
	bgfx::dbgTextPrintf(0, 1, 0x4f, "Physics debug draw is on");
	bgfx::dbgTextPrintf(0, 1, 0x4f, "Drawing %d lines, dropping %d", pos / 2, dropped);
	bgfx::setViewTransform(Core::Engine::Get().GetSystem<Graphics::RenderSystem>()->GetCurrentViewID(), view.data, projection.data);
	bgfx::setVertexBuffer(vb, static_cast<uint32_t>(pos));
	bgfx::submit(Core::Engine::Get().GetSystem<Graphics::RenderSystem>()->GetCurrentViewID(), prog->GetHandle());

	dropped = 0;
	pos = 0;
}