#pragma once
#include <vector>
#include "Graphics/VertexDeclarations.h"

namespace Blueshift {
	namespace Graphics {
		namespace Model {
			namespace Loader {
				//woo that's a lot of namespaces
				bool LoadOBJ(uint8_t* data, std::vector<OBJVertex>& VertexData, std::vector<uint16_t>& IndexData);
			}
		}
	}
} 