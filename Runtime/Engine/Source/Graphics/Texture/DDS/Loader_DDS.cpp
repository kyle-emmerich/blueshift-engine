#include "Graphics/Texture/DDS/Loader.h"

using namespace Blueshift;
using namespace Graphics;
using namespace Texture;
using namespace DDS;

Loader::Loader(Storage::File* f)
	: ILoader(f) {
	total_progress.store(file->GetLength());
}

void Loader::Load() {
	//This is pretty much handled by bgfx, so just shove it into a Texture
	asset = new Texture(file, false);
	progress.store(total_progress);
}