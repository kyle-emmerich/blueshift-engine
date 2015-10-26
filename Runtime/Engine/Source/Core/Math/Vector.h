#pragma once
#include <cmath>
#include <ostream>

namespace Blueshift {
	namespace Core {
		namespace Math {

			template<size_t n, typename T = double>
			struct Vector {
			public:
				T data[n];
				constexpr explicit Vector(T s) {
					for (size_t i = 0; i < n; i++) { data[i] = s; }
				}

				constexpr T Length() const {
					T x; for (size_t i = 0; i < n; i++) { x += data[i] * data[i]; }
					return sqrt(x);
				}
				constexpr T SquaredLength() const {
					T x; for (size_t i = 0; i < n; i++) { x += data[i] * data[i]; }
					return x;
				}
			};

			template<typename T>
			struct Vector<2, T> {
				union { T data[2]; struct { T X, Y; }; };
				constexpr Vector(T x, T y)
					: X(x), Y(y) {}
				constexpr explicit Vector(T s)
					: X(s), Y(s) {}
				constexpr explicit Vector()
					: X(0), Y(0) {}
				constexpr explicit Vector(Vector<3, T>& Other)
					: X(Other.X), Y(Other.Y) {}
				constexpr explicit Vector(Vector<4, T>& Other)
					: X(Other.X), Y(Other.Y) {}

				constexpr T Length() const {
					return sqrt(X * X + Y * Y);
				}
				constexpr T SquaredLength() const {
					return X * X + Y * Y;
				}
			};

			template<typename T>
			struct Vector<3, T> {
				union { T data[3]; struct { T X, Y, Z; }; Vector<2, T> XY; };
				constexpr Vector(T x, T y, T z)
					: X(x), Y(y), Z(z) {}
				constexpr explicit Vector(T s)
					: X(s), Y(s), Z(s) {}
				constexpr explicit Vector()
					: X(0), Y(0), Z(0) {}
				constexpr explicit Vector(Vector<2, T>& Other)
					: X(Other.X), Y(Other.Y), Z(0) {}
				constexpr explicit Vector(Vector<4, T>& Other)
					: X(Other.X), Y(Other.Y), Z(Other.Z) {}

				constexpr T Length() const {
					return sqrt(X * X + Y * Y + Z * Z);
				}
				constexpr T SquaredLength() const {
					return X * X + Y * Y + Z * Z;
				}
			};

			template<typename T>
			struct Vector<4, T> {
				union { T data[4]; struct { T X, Y, Z, W; }; Vector<2, T> XY; Vector<3, T> XYZ; };
				constexpr Vector(T x, T y, T z, T w)
					: X(x), Y(y), Z(z), W(w) {}
				constexpr explicit Vector(T s)
					: X(s), Y(s), Z(s), W(s) {}
				constexpr explicit Vector()
					: X(0), Y(0), Z(0), W(0) {}
				constexpr explicit Vector(Vector<2, T>& Other)
					: X(Other.X), Y(Other.Y), Z(0) {}
				constexpr explicit Vector(Vector<3, T>& Other)
					: X(Other.X), Y(Other.Y), Z(Other.Z), W(0) {}

				constexpr T Length() const {
					return sqrt(X * X + Y * Y + Z * Z + W * W);
				}
				constexpr T SquaredLength() const {
					return X * X + Y * Y + Z * Z + W * W;
				}
			};

			template<size_t n, typename T>
			inline Vector<n, T> operator+(const Vector<n, T>& a, const Vector<n, T>& b) {
				Vector<n, T> rv;
				for (size_t i = 0; i < n; i++) { rv.data[i] = a.data[i] + b.data[i]; }
				return rv;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator+(const Vector<n, T>& a, const T& b) {
				Vector<n, T> rv;
				for (size_t i = 0; i < n; i++) { rv.data[i] = a.data[i] + b; }
				return rv;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator-(const Vector<n, T>& a, const Vector<n, T>& b) {
				Vector<n, T> rv;
				for (size_t i = 0; i < n; i++) { rv.data[i] = a.data[i] - b.data[i]; }
				return rv;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator-(const Vector<n, T>& a, const T& b) {
				Vector<n, T> rv;
				for (size_t i = 0; i < n; i++) { rv.data[i] = a.data[i] - b; }
				return rv;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator*(const Vector<n, T>& a, const Vector<n, T>& b) {
				Vector<n, T> rv;
				for (size_t i = 0; i < n; i++) { rv.data[i] = a.data[i] * b.data[i]; }
				return rv;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator*(const Vector<n, T>& a, const T& b) {
				Vector<n, T> rv;
				for (size_t i = 0; i < n; i++) { rv.data[i] = a.data[i] * b; }
				return rv;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator/(const Vector<n, T>& a, const Vector<n, T>& b) {
				Vector<n, T> rv;
				for (size_t i = 0; i < n; i++) { rv.data[i] = a.data[i] / b.data[i]; }
				return rv;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator/(const Vector<n, T>& a, const T& b) {
				Vector<n, T> rv;
				for (size_t i = 0; i < n; i++) { rv.data[i] = a.data[i] / b; }
				return rv;
			}

			template<size_t n, typename T>
			inline Vector<n, T> operator+=(Vector<n, T>& a, const Vector<n, T>& b) {
				for (size_t i = 0; i < n; i++) { a.data[i] += b.data[i]; }
				return a;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator+=(Vector<n, T>& a, const T& b) {
				for (size_t i = 0; i < n; i++) { a.data[i] += b; }
				return a;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator-=(Vector<n, T>& a, const Vector<n, T>& b) {
				for (size_t i = 0; i < n; i++) { a.data[i] -= b.data[i]; }
				return a;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator-=(Vector<n, T>& a, const T& b) {
				for (size_t i = 0; i < n; i++) { a.data[i] -= b; }
				return a;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator*=(Vector<n, T>& a, const Vector<n, T>& b) {
				for (size_t i = 0; i < n; i++) { a.data[i] += b.data[i]; }
				return a;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator*=(Vector<n, T>& a, const T& b) {
				for (size_t i = 0; i < n; i++) { a.data[i] *= b; }
				return a;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator/=(Vector<n, T>& a, const Vector<n, T>& b) {
				for (size_t i = 0; i < n; i++) { a.data[i] /= b.data[i]; }
				return a;
			}
			template<size_t n, typename T>
			inline Vector<n, T> operator/=(Vector<n, T>& a, const T& b) {
				for (size_t i = 0; i < n; i++) { a.data[i] /= b; }
				return a;
			}

			typedef Vector<2, double> Vector2; typedef Vector<2, float> Vector2f;
			typedef Vector<3, double> Vector3; typedef Vector<3, float> Vector3f;
			typedef Vector<4, double> Vector4; typedef Vector<4, float> Vector4f;

			template<size_t n, typename T>
			constexpr T DotProduct(const Vector<n, T>& a, const Vector<n, T>& b) {
				T x; for (size_t i = 0; i < n; i++) { x += a.data[i] * b.data[i]; }
				return x;
			}
			template<size_t n, typename T>
			constexpr T AngleBetween(const Vector<n, T>& a, const Vector<n, T>& b) {
				T x; for (size_t i = 0; i < n; i++) { x += a.data[i] * b.data[i]; }
				return acos(x);
			}
			template<size_t n, typename T>
			constexpr T Distance(const Vector<n, T>& a, const Vector<n, T>& b) {
				T x; for (size_t i = 0; i < n; i++) { x += (a.data[i] - b.data[i]) * (a.data[i] - b.data[i]); }
				return sqrt(x);
			}
			template<size_t n, typename T>
			constexpr T SquaredDistance(const Vector<n, T>& a, const Vector<n, T>& b) {
				T x; for (size_t i = 0; i < n; i++) { x += (a.data[i] - b.data[i]) * (a.data[i] - b.data[i]); }
				return x;
			}

			template<typename T>
			constexpr Vector<3, T> CrossProduct(const Vector<3, T>& a, const Vector<3, T>& b) {
				return Vector<3, T>(
					lhs.Y * rhs.Z - lhs.Z * rhs.Y,
					lhs.Z * rhs.X - lhs.X * rhs.Z,
					lhs.X * rhs.Y - lhs.Y * rhs.X
					);
			}

		}
	}
}

template<size_t n, typename T>
inline std::ostream& operator<<(std::ostream& os, const Blueshift::Core::Math::Vector<n, T>& rhs) {
	os << "(" << n << "-vec: ";
	for (size_t i = 0; i < n; i++) {
		os << rhs.data[i];
		if (i < n - 1) {
			os << ", ";
		}
	}
	os << ")";
	return os;
}