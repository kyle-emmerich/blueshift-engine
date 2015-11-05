#pragma once
#include "bgfx/bgfx.h"
#include "Graphics/VertexDeclarations.h"
#include "Graphics/ShaderProgram.h"
#include "Core/Math/AABB.h"
#include "Core/Math/OBB.h"
#include "Core/Math/Sphere.h"

namespace Blueshift {
	namespace Graphics {
		class RenderSystem;

		namespace Model {

			class IMeshData;

			struct MeshBounds {
				Core::Math::AABB<float> BoundingBox;
				Core::Math::OBB<float> OrientedBoundingBox;
				Core::Math::Sphere<float> BoundingSphere;
			};

			struct MeshDataSection {
				size_t VertexStart;
				size_t VertexNumber;

				size_t IndexStart;
				size_t IndexNumber;

				IMeshData* MeshData;
				MeshBounds Bounds;

				inline void Render(RenderSystem* renderer, ShaderProgram* program) {
					MeshData->Render(renderer, program, this);
				}
			};

			class IMeshData {
			public:
				virtual void Render(RenderSystem*, ShaderProgram*, size_t) = 0;
				virtual void Render(RenderSystem*, ShaderProgram*, MeshDataSection*) = 0;

				virtual const MeshDataSection& GetSection(size_t SectionID) const = 0;
				virtual const std::string& GetName() const = 0;
			};

			

		}
	}
}