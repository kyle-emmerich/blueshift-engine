#pragma once
#include "Core/Math/Misc.h"
#include "Core/Math/Ray.h"

namespace Blueshift {
	namespace Core {
		namespace Math {

			template<typename T>
			struct AABB {
				Vector<3, T> Center;
				Vector<3, T> HalfWidth;

				constexpr AABB(Vector<3, T> center, Vector<3, T> halfwidth)
					: Center(center), HalfWidth(halfwidth) {}

				inline Vector<3, T> Min() const {
					return Center - HalfWidth;
				}
				inline Vector<3, T> Max() const {
					return Center + HalfWidth;
				}
				inline Vector<3, T> Extents() const {
					return HalfWidth * 2;
				}

				constexpr T Volume() const {
					return HalfWidth.X * HalfWidth.Y * HalfWidth.Z * 8;
				}
			};

			template<typename T>
			bool Intersection(const AABB& a, const AABB& b) {
				bool x = Absolute(a.Center.X - b.Center.X) <= (a.HalfWidth.X + b.HalfWidth.X);
				bool y = Absolute(a.Center.Y - b.Center.Y) <= (a.HalfWidth.Y + b.HalfWidth.Y);
				bool z = Absolute(a.Center.Z - b.Center.Z) <= (a.HalfWidth.Z + b.HalfWidth.Z);
				return x && y && z;
			}

			template<size_t n, typename T>
			bool Intersection(const Ray_<n, T>& ray, const AABB& a, T& Near, T& Far) {
				T t0 = 0;
				T t1 = ray.MaxDistance;

				Vector<3, T> min = a.Min();
				Vector<3, T> max = a.Max();

				for (size_t i = 0; i < 3; i++) {
					T inv_dir = 1 / ray.Direction.data[i];
					T near = (min.data[i] - ray.Origin.data[i]) * inv_dir;
					T far = (max.data[i] - ray.Origin.data[i]) * inv_dir;

					if (near > far) {
						std::swap(near, far);
					}

					t0 = near > t0 ? near : t0;
					t1 = far < t1 ? far : t1;

					if (t0 > t1) {
						return false;
					}
				}

				Near = t0;
				Far = t1;
				return true;
			}
		}
	}
}