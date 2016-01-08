#pragma once
#include "Core/Math/Vector4.h"
#include "Core/Math/Misc.h"
#include "Core/Math/Ray.h"

namespace Blueshift {
	namespace Core {
		namespace Math {

			struct AABB {
				Vector4 Center;
				Vector4 HalfWidth;

				constexpr AABB() 
					: Center(Vector4()), HalfWidth(Vector4()) { }

				constexpr AABB(Vector4 center, Vector4 halfwidth)
					: Center(center), HalfWidth(halfwidth) {}

				inline Vector4 Min() const {
					return Center - HalfWidth;
				}
				inline Vector4 Max() const {
					return Center + HalfWidth;
				}
				inline Vector4 Extents() const {
					return HalfWidth * 2;
				}

				constexpr float Volume() const {
					return HalfWidth.X * HalfWidth.Y * HalfWidth.Z * 8;
				}
			};

			inline bool Intersection(const AABB& a, const AABB& b) {
				return	Absolute(a.Center.X - b.Center.X) <= (a.HalfWidth.X + b.HalfWidth.X) &&
						Absolute(a.Center.Y - b.Center.Y) <= (a.HalfWidth.Y + b.HalfWidth.Y) &&
						Absolute(a.Center.Z - b.Center.Z) <= (a.HalfWidth.Z + b.HalfWidth.Z);

			}
			/*
			bool Intersection(const Ray& ray, const AABB& a, float& NearOut, float& FarOut) {
				Vector4 bmin = a.Min();
				Vector4 bmax = a.Max();

				float t0 = (bmin.data[0] - ray.Origin.data[0]) * ray.InvDirection.data[0];
				float t1 = (bmax.data[0] - ray.Origin.data[0]) * ray.InvDirection.data[0];
				float t2 = (bmin.data[1] - ray.Origin.data[1]) * ray.InvDirection.data[1];
				float t3 = (bmax.data[1] - ray.Origin.data[1]) * ray.InvDirection.data[1];
				float t4 = (bmin.data[2] - ray.Origin.data[2]) * ray.InvDirection.data[2];
				float t5 = (bmax.data[2] - ray.Origin.data[2]) * ray.InvDirection.data[2];

				float tmin = max(max(min(t0, t1), min(t2, t3)), min(t4, t5));
				float tmax = min(min(max(t0, t1), min(t2, t3)), max(t4, t5));

				if (tmax < 0) {
					return false;
				}
				if (tmin > tmax) {
					return false;
				}

				NearOut = tmin;
				FarOut = tmax;
			}
			*/
		}
	}
}