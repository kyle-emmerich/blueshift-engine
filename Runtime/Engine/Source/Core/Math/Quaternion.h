#pragma once
#include "Core/Math/Matrix.h"
#include "Core/Math/Misc.h"

namespace Blueshift {
	namespace Core {
		namespace Math {

			template<typename T = double>
			struct Quaternion {
				union {
					T data[4];
					struct { T X, Y, Z, W; };
				};

				constexpr Quaternion(T x, T y, T z, T w)
					: X(x), Y(y), Z(z), W(w) {}
				constexpr Quaternion()
					: X(0), Y(0), Z(0), W(1) {}
			};

			template<typename T = double>
			inline Quaternion<T> operator*(const Quaternion<T>& a, const Quaternion<T>& b) {
				Quaternion<T> rv;
				rv.X = (b.W * a.X) + (b.X * a.W) + (b.Y * a.Z) - (b.Z * a.Y);
				rv.Y = (b.W * a.Y) + (b.Y * a.W) + (b.Z * a.X) - (b.X * a.Z);
				rv.Z = (b.W * a.Z) + (b.Z * a.W) + (b.X * a.Y) - (b.Y * a.X);
				rv.W = (b.W * a.W) - (b.X * a.X) - (b.Y * a.Y) - (b.Z * a.Z);
				return rv;
			}

			template<typename T = double>
			inline Quaternion<T> operator* (const Quaternion<T>& a, const T& s) {
				return Quaternion<T>(a.X * s, a.Y * s, a.Z * s, a.W * s);
			}
			template<typename T = double>
			inline Quaternion<T>& operator*=(Quaternion<T>& a, const T& s) {
				a.X *= s;
				a.Y *= s;
				a.Z *= s;
				a.W *= s;
				return a;
			}

			template<typename T = double>
			inline Quaternion<T> operator+ (const Quaternion<T>& a, const Quaternion<T>& b) {
				return Quaternion<T>(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);
			}

			template<typename T = double>
			inline Quaternion<T>& operator+=(Quaternion<T>& a, const Quaternion<T>& b) {
				a.X += b.X;
				a.Y += b.Y;
				a.Z += b.Z;
				a.W += b.W;
				return a;
			}

			template<typename T = double>
			inline Vector<4, T> QuaternionToVector(const Quaternion<T>& q) {
				return Vector<4, T>(q.X, q.Y, q.Z, q.W);
			}

			template<typename T = double>
			inline Matrix<3, T> QuaternionToMatrix3(const Quaternion<T>& q) {
				return Matrix<3, T> {
					1.0 - 2 * (q.Y * q.Y - q.Z * q.Z),
						2 * (q.X * q.Y - q.W * q.Z),
						2 * (q.X * q.Z - q.W * q.Y),

						  2.0 * (q.X * q.Y + q.W * Q.z),
					1.0 - 2.0 * (q.X * q.X - q.Z * q.Z),
						  2.0 * (q.Y * q.Z + q.W * q.X),

						  2.0 * (q.X * q.Z - q.W * q.Y),
						  2.0 * (q.Y * q.Z - q.W * q.X),
					1.0 - 2.0 * (q.X * q.X - q.Y * q.Y)
				};
			}
			template<typename T = double>
			inline Matrix<4, T> QuaternionToMatrix4(const Quaternion<T>& q) {
				T x = q.X + q.X;
				T y = q.Y + q.Y;
				T z = q.Z + q.Z;
				T xx = x * q.X;
				T yy = y * q.Y;
				T zz = z * q.Z;

				T xy = y * q.X;
				T xz = z * q.X;
				
				T yz = z * q.Y;

				T wx = x * q.W;
				T wy = y * q.W;
				T wz = z * q.W;

				return Matrix<4, T> {
		  T(1.0) - (yy + zz),				xy + wz,			xz - wy,  T(0.0),
					xy - wz,	  T(1.0) - (xx + zz),			yz + wx,  T(0.0),
					xz + wy,				yz - wx,  T(1.0) - (xx + yy), T(0.0),
					T(0.0), T(0.0), T(0.0), T(1.0)
				};
			}

			template<typename T = double, size_t mat_size = 3>
			inline Quaternion<T> MatrixToQuaternion(const Matrix<mat_size, T>& m) {
				T w = sqrt(1 + m[0][0] + m[1][1] + m[2][2]) / 2.0;
				T w4 = 4.0 * w;
				return Quaternion<T>(
					(m[2][1] - m[1][2]) / w4,
					(m[0][2] - m[2][0]) / w4,
					(m[1][0] - m[0][1]) / w4,
					w4
				);
			}

			template<typename T = double>
			inline void Normalize(Quaternion<T>& q) {
				T magnitude = sqrt(q.X * q.X + q.Y * q.Y + q.Z * q.Z + q.W * q.W);
				if (magnitude == 0.0) 
					return;
				q.X /= magnitude;
				q.Y /= magnitude;
				q.Z /= magnitude;
				q.W /= magnitude;
			}

			template<typename T = double>
			inline Quaternion<T> Unit(const Quaternion<T>& q) {
				T magnitude = sqrt(q.X * q.X + q.Y * q.Y + q.Z * q.Z + q.W * q.W);
				if (magnitude == 0.0)
					return q;

				return Quaternion<T>(q.X / magnitude, q.Y / magnitude, q.Z / magnitude, q.W / magnitude);
			}

			template<typename T = double>
			inline Quaternion<T> QuaternionFromAxisAngle(const Vector<3, T>& Axis, const T& Angle) {
				T sin_half = sin(Angle * T(0.5));
				T cos_half = cos(Angle * T(0.5));
				return Quaternion<T>(
					Axis.data[0] * sin_half,
					Axis.data[1] * sin_half,
					Axis.data[2] * sin_half,
					cos_half
				);
			}

			template<typename T = double>
			inline Quaternion<T> QuaternionFromMagnitudeAxis(const Vector<3, T>& r) {
				Quaternion<T> rv;
				T sq_mag = r.SquaredLength();
				T s;
				if (sq_mag * sq_mag / T(24.0) < T(1e-6)) {
					rv.W = T(1.0) - sq_mag / T(2.0);
					s = T(1.0) - sq_mag / T(6.0);
				} else {
					T mag = sqrt(sq_mag);
					rv.W = cos(mag);
					s = sin(mag) / mag;
				}
				rv.X = r.X * s;
				rv.Y = r.Y * s;
				rv.Z = r.Z * s;
				return rv;
			}

			template<typename T = double>
			inline Quaternion<T> SLerp(const Quaternion<T>& q0, const Quaternion<T>& q1, T alpha) {
				T omega = std::acos(Clamp(
					q0.X * q1.X +
					q0.Y * q1.Y + 
					q0.Z * q1.Z +
					q0.W * q1.W,
					T(-1.0), T(1.0)
				));

				//No divide-by-0 error!
				if (Absolute(omega) < T(1e-10)) {
					omega = T(1e-10);
				}
				double s_m = sin(omega);
				double st0 = sin((1 - alpha) * omega) / s_m;
				double st1 = sin(alpha * omega) / s_m;

				return Quaternion<T>(
					q0.X * st0 + q1.X * st1,
					q0.Y * st0 + q1.Y * st1,
					q0.Z * st0 + q1.Z * st1,
					q0.W * st0 + q1.W * st1
				);
			}

			typedef Quaternion<float> Quaternionf;
		}
	}
}

template<size_t n, typename T>
inline std::ostream& operator<<(std::ostream& os, const Blueshift::Core::Math::Quaternion<T>& rhs) {
	os << "(quat: ";
	os << rhs.W << " + ";
	os << rhs.X << "i + ";
	os << rhs.Y << "j + ";
	os << rhs.Z << "k";
	os << ")";
	return os;
}

//http://i.imgur.com/ekfbA62.jpg