#pragma once
#include "Core/Math/Vector.h"
#include <stdint.h>

namespace Blueshift {
	namespace Core {
		namespace Math {
			struct Coordinate {
				Vector<3, int64_t> km;
				Vector<3, double> m;
			};
		}
	}
}