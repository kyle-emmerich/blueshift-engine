#pragma once
#include "bgfx/bgfx.h"
#include "Graphics/RenderData.h"

namespace Blueshift {
	namespace Graphics {

		class Framebuffer {
		private:
			bgfx::FrameBufferHandle handle = BGFX_INVALID_HANDLE;
		public:
			 Framebuffer();
			~Framebuffer();
		};

	}
}