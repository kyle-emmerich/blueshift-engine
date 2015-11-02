#pragma once
#include "bgfx/bgfx.h"
#include "btBulletDynamicsCommon.h"
#include "Core/Math/Vector.h"
#include "Core/Math/Color.h"
#include <vector>

namespace Blueshift {
	namespace Physics {
		namespace Interop {

			class DebugDraw : btIDebugDraw {
			private:
				struct debug_vertex {
					Core::Math::Vector3f position;
					Core::Math::ColorRGBA8 color;
				};
				static const size_t max_verts = 4096;
				size_t pos = 0;
				size_t dropped = 0;
				debug_vertex vb_vertices[max_verts];
				
				bgfx::DynamicVertexBufferHandle vb;

				Graphics::Shader* vs;
				Graphics::Shader* fs;
				Graphics::ShaderProgram* prog;
			public:
				DebugDraw();
				virtual ~DebugDraw();

				void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

				void Render();
			};

		}
	}
}