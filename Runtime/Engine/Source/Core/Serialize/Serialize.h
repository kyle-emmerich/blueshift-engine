#pragma once
#include <inttypes.h>

namespace Blueshift {
	namespace Core {
		namespace Serialize {

			struct SerializedString {
				size_t length;
				size_t offset;
			};
			struct SerializedList {
				size_t elem_size;
				uint16_t idx;
			};

		}
	}
}