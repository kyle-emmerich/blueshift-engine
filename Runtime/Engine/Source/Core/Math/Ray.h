#pragma once

namespace Blueshift {
	namespace Core {
		namespace Math {

			template<size_t n, typename T>
			struct Ray_ {
				Vector<n, T> Origin;
				Vector<n, T> Direction;

				T MaxDistance;
			};

			typedef Ray_<3, double> Ray;
			typedef Ray_<3, float> Rayf;
		}
	}
}