#pragma once
#include "Core/Math/Vector.h"
#include "Core/Math/Misc.h"
#include "Core/Math/Ray.h"

namespace Blueshift {
	namespace Core {
		namespace Math {

			template<typename T>
			struct Sphere {
				Vector<3, T> Center;
				T Radius;
			};

		}
	}
}