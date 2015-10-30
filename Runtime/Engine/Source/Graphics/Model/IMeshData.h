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

				MeshBounds Bounds;
			};

			class IMeshData {
			public:
				virtual void Render(RenderSystem* renderer, ShaderProgram*, size_t SectionID) = 0;
				virtual void Render(RenderSystem* renderer, ShaderProgram*, MeshDataSection* Section) = 0;

				virtual const MeshDataSection& GetSection(size_t SectionID) const = 0;
				virtual const std::string& GetName() const = 0;
			};

			

		}
	}
}