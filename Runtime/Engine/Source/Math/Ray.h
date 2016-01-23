#pragma once
#include "Math/Vector4.h"

namespace Blueshift {
	namespace Math {

		struct Ray {
			Vector4 Origin;
			Vector4 Direction;
			Vector4 InvDirection;

			float MaxDistance;

			Ray(Vector4 origin, Vector4 direction, float max_distance)
				: Origin(origin), Direction(direction), MaxDistance(max_distance) {
				InvDirection = 1.0f / Direction;
			}

			void SetDirection(Vector4 direction) {
				Direction = direction;
				InvDirection = 1.0f / Direction;
			}
		};
	}
}