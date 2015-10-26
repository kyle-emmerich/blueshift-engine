#pragma once
#include "bgfx/bgfx.h"
#include "Graphics/VertexDeclarations.h"
#include "Graphics/ShaderProgram.h"

namespace Blueshift {
	namespace Graphics {
		namespace Model {

			class IMeshData {
			public:
				virtual void Render(ShaderProgram*, size_t SectionID) = 0;
				virtual void Render(ShaderProgram*, MeshDataSection* Section) = 0;
			};

			struct MeshDataSection {
				size_t VertexStart;
				size_t VertexNumber;

				size_t IndexStart;
				size_t IndexNumber;
			};

		}
	}
}