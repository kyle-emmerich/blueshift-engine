#pragma once
#include <string>

namespace Blueshift {

	namespace Graphics {
		class RenderSystem;
	}

	namespace Platform {

		struct DisplayInfo {
			DisplayInfo() {}

			uint32_t Index;
			std::string AdapterName;

			int32_t X;
			int32_t Y;

			int32_t Width;
			int32_t Height;

			void* NativeHandle;

			static void EnumerateDisplays(Blueshift::Graphics::RenderSystem* engine);
		};

	}
}