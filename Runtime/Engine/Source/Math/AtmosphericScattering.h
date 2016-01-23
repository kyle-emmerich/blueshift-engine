#pragma once
#include <cmath>

namespace Blueshift {
	namespace Core {
		namespace Math {
			namespace AtmosphericScattering {

				//u should be between 0.7 and 0.85
				float AsymmetryFactor(float u) {
					float x = (5.0f / 9.0f) * u + (125.0f / 729.0f) * (u*u*u) + sqrt((64.0f / 27.0f) - (325.0f / 243.0f)*(u*u) + (1250.0f / 2187.0f)*(u*u*u*u));
					x = pow(x, 1.0f / 3.0f);
					return (5.0f / 9.0f) * u - (((4.0f / 3.0f) - (25.0f / 81.0f) * (u * u)) / x) + x;
				}

			}
		}
	}
}