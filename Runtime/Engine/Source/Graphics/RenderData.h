#pragma once
#include "bgfx/bgfx.h"

namespace Blueshift {
	namespace Graphics {

		typedef const bgfx::Memory* RenderData;
		inline RenderData AllocateRenderData(size_t size) {
			return bgfx::alloc(static_cast<uint32_t>(size));
		}
	}
}