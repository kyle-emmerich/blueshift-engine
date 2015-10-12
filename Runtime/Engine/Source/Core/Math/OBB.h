#pragma once
#include "Core/Math/Vector.h"

namespace Blueshift {
	namespace Core {
		namespace Math {

			template<typename T>
			struct OBB {
				Vector<3, T> Center;
				Vector<3, T> HalfWidth;
				Vector<3, T> Axis;
			};

		}
	}
}