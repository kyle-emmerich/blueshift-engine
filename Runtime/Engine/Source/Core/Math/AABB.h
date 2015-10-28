#pragma once
#include "Core/Math/Vector.h"
#include "Core/Math/Misc.h"
#include "Core/Math/Ray.h"

namespace Blueshift {
	namespace Core {
		namespace Math {

			template<typename T>
			struct AABB {
				Vector<3, T> Center;
				Vector<3, T> HalfWidth;

				constexpr AABB() 
					: Center(Vector<3, T>()), HalfWidth(Vector<3, T>()) { }

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
			bool Intersection(const AABB<T>& a, const AABB<T>& b) {
				return	Absolute(a.Center.X - b.Center.X) <= (a.HalfWidth.X + b.HalfWidth.X) &&
						Absolute(a.Center.Y - b.Center.Y) <= (a.HalfWidth.Y + b.HalfWidth.Y) &&
						Absolute(a.Center.Z - b.Center.Z) <= (a.HalfWidth.Z + b.HalfWidth.Z);

			}

			template<typename T>
			bool Intersection(const Ray_<3, T>& ray, const AABB<T>& a, T& NearOut, T& FarOut) {
				Vector<3, T> bmin = a.Min();
				Vector<3, T> bmax = a.Max();

				T t0 = (bmin.data[0] - ray.Origin.data[0]) * ray.InvDirection.data[0];
				T t1 = (bmax.data[0] - ray.Origin.data[0]) * ray.InvDirection.data[0];
				T t2 = (bmin.data[1] - ray.Origin.data[1]) * ray.InvDirection.data[1];
				T t3 = (bmax.data[1] - ray.Origin.data[1]) * ray.InvDirection.data[1];
				T t4 = (bmin.data[2] - ray.Origin.data[2]) * ray.InvDirection.data[2];
				T t5 = (bmax.data[2] - ray.Origin.data[2]) * ray.InvDirection.data[2];

				T tmin = max(max(min(t0, t1), min(t2, t3)), min(t4, t5));
				T tmax = min(min(max(t0, t1), min(t2, t3)), max(t4, t5));

				if (tmax < 0) {
					return false;
				}
				if (tmin > tmax) {
					return false;
				}

				NearOut = tmin;
				FarOut = tmax;
			}
		}
	}
}