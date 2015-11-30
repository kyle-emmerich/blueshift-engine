#pragma once
#include "Core/Math/Vector.h"
#include <stdint.h>

namespace Blueshift {
	namespace Core {
		namespace Math {
			struct Coordinate {
				int64_t X;
				int64_t Y;
				int64_t Z;

				Vector4 m;
			};
		}
	}
}