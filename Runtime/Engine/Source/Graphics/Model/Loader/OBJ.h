#pragma once
#include <vector>
#include "Graphics/Model/StaticMeshData.h"
#include "Graphics/VertexDeclarations.h"

namespace Blueshift {
	namespace Graphics {
		namespace Model {
			typedef uint32_t OBJIndex;
			typedef StaticMeshData<OBJVertex, OBJIndex> OBJMeshData;
			
			namespace Loader {

				

				class OBJ {
				private:
					struct o {
						std::string name;
						bool complete = false;
						std::vector<Core::Math::Vector3f> p;
						std::vector<Core::Math::Vector2f> t;
						std::vector<Core::Math::Vector3f> n;

						std::vector<OBJVertex> verts;
						std::vector<OBJIndex> indices;
					};
					std::vector<std::unique_ptr<o>> objects;
					o* current;

					void process_line(std::string line);
					void process_v(std::string line, size_t offset);
					void process_vt(std::string line, size_t offset);
					void process_vn(std::string line, size_t offset);
					void process_f(std::string line, size_t offset);
					void process_triplet(std::string line, size_t offset);
				
					void to_mesh_data(std::vector<std::unique_ptr<Model::OBJMeshData>>& meshes);
				public:
					 OBJ();
					~OBJ();

					bool Load(std::istream& is, std::vector<std::unique_ptr<Model::OBJMeshData>>& meshes);
					bool Load(uint8_t* data, size_t size, std::vector<std::unique_ptr<Model::OBJMeshData>>& meshes);
				};
			
			}
		}
	}
} 