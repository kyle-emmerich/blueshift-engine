#pragma once
#include "Graphics/Texture/Texture.h"
#include "Storage/ILoader.h"

namespace Blueshift {
	namespace Graphics {
		namespace Texture {
			namespace DDS {
				 
				class Loader : public Storage::ILoader {
				private:
					Texture* asset;
				public:
					Loader(Storage::File* f);
					~Loader() {} //TODO: finish when i get back 

					//Should only be called in loading thread
					virtual void Load() override;
				};
			}
		}
	}
}