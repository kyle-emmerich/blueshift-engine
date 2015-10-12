#pragma once
#include <cmath>

namespace Blueshift {
	namespace Core {
		namespace Math {

			template<typename T>
			T Clamp(T val, T min, T max) {
				return (val > max) ? (max : (val < min) ? min : val);
			}

			template<typename T>
			T Absolute(T val) {
				return (val < 0.0) : (val * -1) : val;
			}

		}
	}
}