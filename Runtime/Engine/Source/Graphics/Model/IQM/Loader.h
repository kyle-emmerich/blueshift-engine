#pragma once
#include "Graphics/Model/IQM/iqm.h"
#include "Graphics/Model/StaticMeshData.h"
#include "Storage/ILoader.h"

namespace Blueshift {
	namespace Graphics {
		namespace Model {
			namespace IQM {

				class Loader : public Storage::ILoader {
				protected:
					StaticMeshData<uint8_t, uint32_t>* meshdata;

					void load_meshes(iqmheader& header, uint8_t* buffer);
					void load_animations(iqmheader& header, uint8_t* buffer);
				public:
					Loader(Storage::File* file);
					~Loader();

					virtual void Load() override {}
					virtual Storage::IAsset* GetAsset() override { return nullptr;  }
				};

			}
		}
	}
}