#pragma once
#include "Core/Math/Vector.h"

namespace Blueshift {
	namespace Core {
		namespace Math {

			template<typename T>
			struct OBB {
				Vector4 Center;
				Vector4 HalfWidth;
				Vector4 Axis;
			};

		}
	}
}