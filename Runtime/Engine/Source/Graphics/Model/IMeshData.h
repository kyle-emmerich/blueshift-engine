#pragma once
#include "bgfx/bgfx.h"
#include "Graphics/VertexDeclarations.h"
#include "Graphics/ShaderProgram.h"
#include "Core/Math/Math.h"
#include "Storage/IAsset.h"

namespace Blueshift {
	namespace Graphics {
		class RenderSystem;

		namespace Model {

			class IMeshData;

			struct MeshBounds {
				Core::Math::AABB BoundingBox;
				float BoundingRadius;
			};

			struct MeshDataSection {
				size_t VertexStart;
				size_t VertexNumber;

				size_t IndexStart;
				size_t IndexNumber;

				IMeshData* MeshData;
				MeshBounds Bounds;
			};

			struct MeshBone {
				MeshBone* parent;
				Core::Math::Matrix4 frame;
				Core::Math::Matrix4 inverse_frame;
			};

			class IMeshData : public Storage::IAsset {
			public:
				virtual void Render(RenderSystem*, ShaderProgram*, size_t) = 0;
				virtual void Render(RenderSystem*, ShaderProgram*, MeshDataSection*) = 0;

				virtual MeshDataSection& GetSection(size_t SectionID) = 0;
				virtual const MeshDataSection& GetSection(size_t SectionID) const = 0;
				virtual size_t GetNumSections() const = 0;

				virtual MeshBone& GetBone(size_t BoneID) = 0;
				virtual const MeshBone& GetBone(size_t BoneID) const = 0;
				virtual size_t GetNumBones() const = 0;
			};

			

		}
	}
}