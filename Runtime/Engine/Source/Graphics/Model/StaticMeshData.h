#pragma once
#include "Graphics/Model/IMeshData.h"
#include "Core/Engine.h"

namespace Blueshift {
	namespace Graphics {
		namespace Model {

			template<typename VertexType = StaticVertex,
					 typename IndexType = uint16_t>
			class StaticMeshData : public IMeshData {
			protected:
				bgfx::VertexBufferHandle vertex_buf;
				bgfx::IndexBufferHandle index_buf;

				VertexType* vertex_data;
				size_t num_vertices;
				IndexType* index_data;
				size_t num_indices;

				std::vector<MeshDataSection> sections;
			public:
				StaticMeshData() = delete;
				StaticMeshData(
					VertexType* Vertices, size_t NumVertices,
					IndexType* Indices, size_t NumIndices)
					: vertex_data(Vertices), num_vertices(NumVertices),
					  index_data(Indices), num_indices(NumIndices) {

					//Are we using 32-bit indices?
					uint16_t index_flags = BGFX_BUFFER_NONE;
					if (typeid(IndexType) == typeid(uint32_t)) {
						//We are, set the flag
						index_flags = BGFX_BUFFER_INDEX32;
					} else if (typeid(IndexType) == typeid(uint16_t)) {
						//Do nothing, no flags to set for 16-bit indices.
					} else {
						//TODO: Throw an error for an unsupported index type?
						//		This can probably be done at compile-time using newer
						//		C++ features. A future consideration.
					}
					
					vertex_buf = bgfx::createVertexBuffer(
						bgfx::makeRef(vertex_data, num_vertices * sizeof(VertexType)),
						VertexType::decl,
						BGFX_BUFFER_NONE
					);
					index_buf = bgfx::createIndexBuffer(
						bgfx::makeRef(index_data, num_indices * sizeof(IndexType)),
						index_flags
					);

					//SectionID=0 is always the whole data buffer unless
					//modified later.
					sections.push_back({
						0, num_vertices,
						0, num_indices
					});
				}
				//It doesn't make much sense to have copy/move on a static data class.
				//You can only use it to render after instantiation, so just pass it
				//as a reference or pointer.
				StaticMeshData(StaticMeshData& data) = delete;
				StaticMeshData(StaticMeshData&& data) = delete;

				virtual ~StaticMeshData() {
					if (vertex_buf != BGFX_INVALID_HANDLE) {
						bgfx::destroyVertexBuffer(vertex_buf);
						vertex_buf = BGFX_INVALID_HANDLE;
					}
					if (index_buf != BGFX_INVALID_HANDLE) {
						bgfx::destroyIndexBuffer(index_buf);
						index_buf = BGFX_INVALID_HANDLE;
					}

					if (vertex_data != nullptr) {
						delete[] vertex_data;
						vertex_data = nullptr;
					}
					
					if (index_data != nullptr) {
						delete[] index_data;
						index_data = nullptr;
					}
				}

				size_t AddSection(size_t VertexStart, size_t VertexNumber, size_t IndexStart, size_t IndexNumber) {
					size_t index = sections.size();
					sections.push_back({
						VertexStart, VertexNumber,
						IndexStart, IndexNumber
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
					bgfx::setVertexBuffer(vertex_buf, Section->VertexStart, Section->VertexNumber);
					bgfx::setIndexBuffer(index_buf, Section->IndexStart, Section->IndexNumber);
					bgfx::submit(Core::Engine::GetRenderSystem().GetCurrentViewID(), program->GetHandle());
				}
			};

		}
	}
}