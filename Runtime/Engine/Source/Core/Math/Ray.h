#pragma once
#include "Core/Math/Vector.h"

namespace Blueshift {
	namespace Core {
		namespace Math {

			template<size_t n, typename T>
			struct Ray_ {
				Vector<n, T> Origin;
				Vector<n, T> Direction;
				Vector<n, T> InvDirection;

				T MaxDistance;

				Ray_(Vector<n, T> origin, Vector<n, T> direction, T max_distance)
					: Origin(origin), Direction(direction), MaxDistance(max_distance) {
					InvDirection = T(1) / Direction;
				}

				void SetDirection(Vector<n, T> direction) {
					Direction = direction;
					InvDirection = T(1) / Direction;
				}
			};

			typedef Ray_<3, double> Ray;
			typedef Ray_<3, float> Rayf;
		}
	}
}