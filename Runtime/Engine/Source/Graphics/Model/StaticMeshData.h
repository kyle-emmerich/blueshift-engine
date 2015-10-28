#pragma once
#include "Graphics/Model/IMeshData.h"
#include "Core/Engine.h"

namespace Blueshift {
	namespace Graphics {
		namespace Model {

			template<typename VertexT = StaticVertex,
					 typename IndexT = uint16_t>
			class StaticMeshData : public IMeshData {
			protected:
				bgfx::VertexBufferHandle vertex_buf;
				bgfx::IndexBufferHandle index_buf;

				VertexT* vertex_data;
				IndexT* index_data;
				size_t num_vertices;
				size_t num_indices;

				std::string name;

				std::vector<MeshDataSection> sections;
			public:
				StaticMeshData() = delete;
				StaticMeshData(
					std::string name,
					VertexT* Vertices, size_t NumVertices, VertexType& Type,
					IndexT* Indices, size_t NumIndices, MeshBounds Bounds)
					: num_vertices(NumVertices), num_indices(NumIndices) {

					vertex_data = new VertexT[NumVertices * Type.getStride()];
					index_data = new IndexT[NumIndices * sizeof(IndexT)];

					memcpy(vertex_data, Vertices, NumVertices * Type.getStride());
					memcpy(index_data, Indices, NumIndices * sizeof(IndexT));

					//Are we using 32-bit indices?
					uint16_t index_flags = BGFX_BUFFER_NONE;
					if (sizeof(IndexT) == sizeof(uint32_t)) {
						//We are, set the flag
						index_flags = BGFX_BUFFER_INDEX32;
					} else if (sizeof(IndexT) == sizeof(uint16_t)) {
						//Do nothing, no flags to set for 16-bit indices.
					} else {
						//TODO: Throw an error for an unsupported index type?
						//		This can probably be done at compile-time using newer
						//		C++ features. A future consideration.
					}
					
					vertex_buf = bgfx::createVertexBuffer(
						bgfx::makeRef(vertex_data, static_cast<uint32_t>(num_vertices * sizeof(VertexT))),
						Type,
						BGFX_BUFFER_NONE
					);
					index_buf = bgfx::createIndexBuffer(
						bgfx::makeRef(index_data, static_cast<uint32_t>(num_indices * sizeof(IndexT))),
						index_flags
					);

					//SectionID=0 is always the whole data buffer unless
					//modified later.
					sections.push_back({
						0, num_vertices,
						0, num_indices,
						Bounds
					});
				}
				//It doesn't make much sense to have copy/move on a static data class.
				//You can only use it to render after instantiation, so just pass it
				//as a reference or pointer.
				StaticMeshData(const StaticMeshData& data) = delete;
				StaticMeshData(const StaticMeshData&& data) = delete;

				virtual ~StaticMeshData() {
					if (vertex_buf.idx != bgfx::invalidHandle) {
						bgfx::destroyVertexBuffer(vertex_buf);
						vertex_buf = BGFX_INVALID_HANDLE;
					}
					if (index_buf.idx != bgfx::invalidHandle) {
						bgfx::destroyIndexBuffer(index_buf);
						index_buf = BGFX_INVALID_HANDLE;
					}

					if (vertex_data != nullptr) {
						delete[] vertex_data;
					}
					if (index_data != nullptr) {
						delete[] index_data;
					}
				}

				size_t AddSection(size_t VertexStart, size_t VertexNumber, size_t IndexStart, size_t IndexNumber, MeshBounds Bounds) {
					size_t index = sections.size();
					sections.push_back({
						VertexStart, VertexNumber,
						IndexStart, IndexNumber,
						Bounds
					});
					return index;
				}


				inline virtual void Render(ShaderProgram* program, size_t SectionID = 0) {
					if (SectionID >= sections.size()) {
						throw 0; //TODO: Make an exception type for this.
						//Should indicate section is out of bounds.
					}
					MeshDataSection* section = &sections[SectionID];
					Render(program, section);
				}
				inline virtual void Render(ShaderProgram* program, MeshDataSection* Section) {
					//TODO: make sure section isn't null...
					//TODO: make sure program is valid
					//TODO: replace programhandle with api-friendly wrap
					bgfx::setVertexBuffer(vertex_buf, static_cast<uint32_t>(Section->VertexStart), static_cast<uint32_t>(Section->VertexNumber));
					bgfx::setIndexBuffer(index_buf, static_cast<uint32_t>(Section->IndexStart), static_cast<uint32_t>(Section->IndexNumber));
					bgfx::submit(Core::Engine::Get().GetRenderSystem().GetCurrentViewID(), program->GetHandle());
				}

				inline virtual const std::string& GetName() const {
					return name;
				}
				inline virtual const MeshDataSection& GetSection(size_t SectionID) const {
					return sections[SectionID];
				}
			};

		}
	}
}