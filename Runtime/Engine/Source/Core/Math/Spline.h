#pragma once
#include <vector>
#include <cmath>
#include "Core/Math/Polynomial.h"
#include "Core/Math/Misc.h"

namespace Blueshift {
	namespace Core {
		namespace Math {

			template<size_t n, typename T>
			class Spline {
			private:
				Polynomial<T> dimensions[n];
				Polynomial<T> derivatives[n];
				bool dirty;

				std::vector<Vector<n, T>> points;

				inline void ensure_coefficients() {
					if (!dirty) {
						return;
					}
					dirty = false;

					//the order is the number of points involved; start, end, and all controls.
					size_t o = points.size();
					for (size_t dimension; dimension < n; dimension++) {
						dimensions[dimension].coefficients.clear();
						for (size_t j = 0; j < o; j++) {
							T sum = 0;
							for (size_t i = 0; i < j; i++) {
								sum += (
									std::pow(-1, i + j) * points[i].data[dimension]
									/
									FactorialLookup[i] * FactorialLookup[j - i]
								);
							}
							dimensions[dimension].coefficients.push_back((FactorialLookup[o] / FactorialLookup[o - j]) * sum);
						}
						derivatives[dimension] = dimensions[dimension].Derivative();
					}
				}
			public:
				inline Spline() : dirty(true) { }
				inline Spline(std::initializer_list<Vector<n, T>> list) {
					for (auto it : list) {
						points.push_back((*it));
					}
				}
				~Spline() {}

				inline Vector<n, T> Evaluate(T t) {
					ensure_coefficients();
					Vector<n, T> out;
					for (int i = 0; i < n; i++) {
						out.data[i] = dimensions[i].Evaluate(t);
					}
					return out;
				}
				inline Vector<n, T> TangentTo(T t) {
					ensure_coefficients();
					Vector<n, T> out;
					for (int i = 0; i < n; i++) {
						out.data[i] = derivatives[i].Evaluate(t);
					}
					return out;
				}
	
				inline Vector<n, T>& operator[](size_t i) {
					return points[i];
				}
				inline const Vector<n, T>& operator[](size_t i) const {
					return points[i];
				}
			};

		}
	}
}