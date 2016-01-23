#pragma once
#include "Math/Matrix4.h"
#include "Math/Vector4.h"
#include "Math/Misc.h"

namespace Blueshift {
	namespace Math {

		/*! \class Quaternion
			\brief Represents a 3-dimensional rotation using four complex numbers; you don't have to understand it to use it.

			Quaternions can be best explained related to an axis and a rotation around that axis, but this is not technically 
			correct. The important part is that rotations may be combined by multiplying two quaternions.
		*/
		class Quaternion {
		public:
			union {
				float data[4];
				__m128 mm;
				Vector4 vec;
				struct { float X, Y, Z, W; };
			};

			constexpr Quaternion(float x, float y, float z, float w)
				: X(x), Y(y), Z(z), W(w) {}
			constexpr Quaternion()
				: X(0), Y(0), Z(0), W(1) {}
		};

		/*! \relates Quaternion
			\brief Multiply two quaternions to combine the rotations; not commutative (a*b != b*a).
		*/
		inline Quaternion operator*(const Quaternion& a, const Quaternion& b) {
			Quaternion rv;
			rv.X = (b.W * a.X) + (b.X * a.W) + (b.Y * a.Z) - (b.Z * a.Y);
			rv.Y = (b.W * a.Y) + (b.Y * a.W) + (b.Z * a.X) - (b.X * a.Z);
			rv.Z = (b.W * a.Z) + (b.Z * a.W) + (b.X * a.Y) - (b.Y * a.X);
			rv.W = (b.W * a.W) - (b.X * a.X) - (b.Y * a.Y) - (b.Z * a.Z);
			return rv;
		}

		/*! \relates Quaternion
			\brief Multiply two quaternions to combine the rotations; not commutative (a*b != b*a).
		*/
		inline Quaternion operator* (const Quaternion& a, const float& s) {
			Quaternion rv;
			rv.mm = _mm_mul_ps(a.mm, _mm_load1_ps(&s));
			return rv;
		}

		/*! \relates Quaternion
			\brief Multiply two quaternions to combine the rotations; not commutative (a*b != b*a).
		*/
		inline Quaternion& operator*=(Quaternion& a, const float& s) {
			a.mm = _mm_mul_ps(a.mm, _mm_load1_ps(&s));
			return a;
		}

		inline Quaternion operator+ (const Quaternion& a, const Quaternion& b) {
			return Quaternion(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);
		}

		inline Quaternion& operator+=(Quaternion& a, const Quaternion& b) {
			a.X += b.X;
			a.Y += b.Y;
			a.Z += b.Z;
			a.W += b.W;
			return a;
		}

		/*! \relates Quaternion
			\brief Converts a quaternion to a vector4; this is a raw conversion, no math applied.
			\sa Vector4
		*/
		inline Vector4 QuaternionToVector4(const Quaternion& q) {
			return q.vec;
		}

		/*!	\relates Quaternion
			\brief Converts a quaternion to a rotation matrix with 0,0,0 translation.
			\sa Matrix4
		*/
		inline Matrix4 QuaternionToMatrix4(const Quaternion& q) {
			float x = q.X + q.X;
			float y = q.Y + q.Y;
			float z = q.Z + q.Z;
			float xx = x * q.X;
			float yy = y * q.Y;
			float zz = z * q.Z;

			float xy = y * q.X;
			float xz = z * q.X;
				
			float yz = z * q.Y;

			float wx = x * q.W;
			float wy = y * q.W;
			float wz = z * q.W;

			Matrix4 out {
		1.0f - (yy + zz),				xy + wz,			xz - wy,	0.0f,
				xy - wz,	    1.0f - (xx + zz),			yz + wx,	0.0f,
				xz + wy,				yz - wx,    1.0f - (xx + yy),	0.0f,
				0.0f,					0.0f,				0.0f,		1.0f
			};
			return out;
		}

		/*!	\relates Quaternion
			\brief Converts a rotation matrix to a quaternion; translation component is ignored.
			\sa Matrix4
		*/
		inline Quaternion Matrix4ToQuaternion(const Matrix4& m) {
			float w = sqrt(1.0f + m[0][0] + m[1][1] + m[2][2]) / 2.0f;
			float w4 = 4.0f * w;
			return Quaternion(
				(m[2][1] - m[1][2]) / w4,
				(m[0][2] - m[2][0]) / w4,
				(m[1][0] - m[0][1]) / w4,
				w4
			);
		}

		/*! \relates Quaternion
			\brief Normalizes a quaternion; non-unit quaternions are not valid, so use this to combat floating point errors.
		*/
		inline void Normalize(Quaternion& q) {
			float magnitude = sqrt(q.X * q.X + q.Y * q.Y + q.Z * q.Z + q.W * q.W);
			if (magnitude == 0.0) 
				return;
			q.X /= magnitude;
			q.Y /= magnitude;
			q.Z /= magnitude;
			q.W /= magnitude;
		}

		/*!	\relates Quaternion
			\brief Returns a normalized version of the quaternion copied into a new instance.
		*/
		inline Quaternion Unit(const Quaternion& q) {
			float magnitude = sqrt(q.X * q.X + q.Y * q.Y + q.Z * q.Z + q.W * q.W);
			if (magnitude == 0.0)
				return q;

			return Quaternion(q.X / magnitude, q.Y / magnitude, q.Z / magnitude, q.W / magnitude);
		}

		/*! \relates Quaternion
			\brief Creates a new quaternion from an axis and an angle in radians.
		*/
		inline Quaternion QuaternionFromAxisAngle(const Vector4& Axis, const float& Angle) {
			float sin_half = sin(Angle * 0.5f);
			float cos_half = cos(Angle * 0.5f);
			return Quaternion (
				Axis.data[0] * sin_half,
				Axis.data[1] * sin_half,
				Axis.data[2] * sin_half,
				cos_half
			);
		}

		/*! \relates Quaternion
			\brief Creates a new quaternion from an axis whose magnitude represents the amount of counter-clockwise rotation in radians.
		*/
		inline Quaternion QuaternionFromMagnitudeAxis(const Vector4& r) {
			Quaternion rv;
			float sq_mag = LengthSquared(r);
			float s;
			if (sq_mag * sq_mag / 24.0f < 1e-6f) {
				rv.W = 1.0f - sq_mag / 2.0f;
				s = 1.0f - sq_mag / 6.0f;
			} else {
				float mag = sqrt(sq_mag);
				rv.W = cos(mag);
				s = sin(mag) / mag;
			}
			rv.X = r.X * s;
			rv.Y = r.Y * s;
			rv.Z = r.Z * s;
			return rv;
		}

		/*!	\relates Quaternion
			\brief Creates a new quaternion from a direction and an upwards vector.

			Thanks, CPML. I don't know why this works.
			https://github.com/excessive/cpml/blob/master/modules/quat.lua#L286
		*/
		inline Quaternion QuaternionFromDirection(const Vector4& direction, const Vector4& up) {
			Vector4 axis = CrossProduct(direction, up);
			float dot = DotProduct(up, direction);
			return Quaternion(axis.X, axis.Y, axis.Z, dot + 1.0f);
		}

		/*!	\relates Quaternion
			\brief Returns an interpolated rotation between q0 and q1 by the fraction alpha; a unit vector rotated by this would follow the surface of a sphere.
		*/
		inline Quaternion SLerp(const Quaternion& q0, const Quaternion& q1, float alpha) {
			float omega = std::acos(Clamp(
				q0.X * q1.X +
				q0.Y * q1.Y + 
				q0.Z * q1.Z +
				q0.W * q1.W,
				-1.0f, 1.0f
			));

			//No divide-by-0 error!
			if (Absolute(omega) < 1e-10f) {
				omega = 1e-10f;
			}
			float s_m = sin(omega);
			float st0 = sin((1.0f - alpha) * omega) / s_m;
			float st1 = sin(alpha * omega) / s_m;

			return Quaternion(
				q0.X * st0 + q1.X * st1,
				q0.Y * st0 + q1.Y * st1,
				q0.Z * st0 + q1.Z * st1,
				q0.W * st0 + q1.W * st1
			);
		}
	}
}

inline std::ostream& operator<<(std::ostream& os, const Blueshift::Math::Quaternion& rhs) {
	os << "(quat: ";
	os << rhs.W << " + ";
	os << rhs.X << "i + ";
	os << rhs.Y << "j + ";
	os << rhs.Z << "k";
	os << ")";
	return os;
}