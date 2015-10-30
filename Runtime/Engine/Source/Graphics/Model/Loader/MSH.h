#pragma once
#include "Graphics/Model/StaticMeshData.h"
#include "Graphics/VertexDeclarations.h"

namespace Blueshift {
	namespace Graphics {
		namespace Model {
			namespace Loader {

				class MSH {
				private:

				public:

					void Load(std::istream& is, std::vector<StaticMeshData<>*>& meshes);
				};

			}
		}
	}
}