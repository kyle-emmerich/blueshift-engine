#pragma once
#include <cmath>
#include <ostream>
#include <istream>
#include <initializer_list>


#define BLUESHIFT_OPTIMIZE_SIMD
#define BLUESHIFT_OPTIMIZE_SSE4_1

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
				Vector(std::initializer_list&& list) {
					size_t i = 0;
					for (T v : list) {
						if (i >= n) {
							break;
						}
						data[i++] = v;
					}
				}
				Vector() = default;

				constexpr T Length() const {
					T x; for (size_t i = 0; i < n; i++) { x += data[i] * data[i]; }
					return sqrt(x);
				}
				constexpr T SquaredLength() const {
					T x; for (size_t i = 0; i < n; i++) { x += data[i] * data[i]; }
					return x;
				}

				inline T& operator[](const size_t& idx) {
					static_assert(idx < n, "Vector subscript index out of bounds");
					return data[idx];
				}
				inline const T& operator[](const size_t& idx) const {
					static_assert(idx < n, "Vector subscript index out of bounds");
					return data[idx];
				}
			};

			template<typename T>
			struct Vector<2, T> {
				union { T data[2]; struct { T X, Y; }; };
				constexpr Vector(T x, T y)
					: X(x), Y(y) {}
				constexpr explicit Vector(T s)
					: X(s), Y(s) {}
				constexpr explicit Vector(Vector<3, T>& Other)
					: X(Other.X), Y(Other.Y) {}
				constexpr explicit Vector(Vector<4, T>& Other)
					: X(Other.X), Y(Other.Y) {}
				Vector() = default;

				constexpr T Length() const {
					return sqrt(X * X + Y * Y);
				}
				constexpr T SquaredLength() const {
					return X * X + Y * Y;
				}

				inline T& operator[](const size_t& idx) {
					static_assert(idx < 2, "Vector subscript index out of bounds");
					return data[idx];
				}
				inline const T& operator[](const size_t& idx) const {
					static_assert(idx < 2, "Vector subscript index out of bounds");
					return data[idx];
				}
			};

			template<typename T>
			struct Vector<3, T> {
				union { T data[3]; struct { T X, Y, Z; }; Vector<2, T> XY; };
				constexpr Vector(T x, T y, T z)
					: X(x), Y(y), Z(z) {}
				constexpr explicit Vector(T s)
					: X(s), Y(s), Z(s) {}
				constexpr explicit Vector(Vector<2, T>& Other)
					: X(Other.X), Y(Other.Y), Z(0) {}
				constexpr explicit Vector(Vector<4, T>& Other)
					: X(Other.X), Y(Other.Y), Z(Other.Z) {}
				Vector() = default;

				constexpr T Length() const {
					return sqrt(X * X + Y * Y + Z * Z);
				}
				constexpr T SquaredLength() const {
					return X * X + Y * Y + Z * Z;
				}

				inline T& operator[](const size_t& idx) {
					static_assert(idx < 3, "Vector subscript index out of bounds");
					return data[idx];
				}
				inline const T& operator[](const size_t& idx) const {
					static_assert(idx < 3, "Vector subscript index out of bounds");
					return data[idx];
				}
			};

			template<typename T>
			struct Vector<4, T> {
				union { T data[4]; struct { T X, Y, Z, W; }; Vector<2, T> XY; Vector<3, T> XYZ; };
				constexpr Vector(T x, T y, T z, T w)
					: X(x), Y(y), Z(z), W(w) {}
				constexpr explicit Vector(T s)
					: X(s), Y(s), Z(s), W(s) {}
				constexpr explicit Vector(Vector<2, T>& Other)
					: X(Other.X), Y(Other.Y), Z(0) {}
				constexpr explicit Vector(Vector<3, T>& Other)
					: X(Other.X), Y(Other.Y), Z(Other.Z), W(0) {}
				Vector() = default;

				constexpr T Length() const {
					return sqrt(X * X + Y * Y + Z * Z + W * W);
				}
				constexpr T SquaredLength() const {
					return X * X + Y * Y + Z * Z + W * W;
				}

				inline T& operator[](const size_t& idx) {
					return data[idx];
				}
				inline const T& operator[](const size_t& idx) const {
					return data[idx];
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

			template<size_t n, typename T>
			T DotProduct(const Vector<n, T>& a, const Vector<n, T>& b) {
				T x = T(0); for (size_t i = 0; i < n; i++) { x += a.data[i] * b.data[i]; }
				return x;
			}
			template<size_t n, typename T>
			constexpr T AngleBetween(const Vector<n, T>& a, const Vector<n, T>& b) {
				T x = T(0); for (size_t i = 0; i < n; i++) { x += a.data[i] * b.data[i]; }
				return acos(x);
			}
			template<size_t n, typename T>
			constexpr T Distance(const Vector<n, T>& a, const Vector<n, T>& b) {
				T x = T(0); for (size_t i = 0; i < n; i++) { x += (a.data[i] - b.data[i]) * (a.data[i] - b.data[i]); }
				return sqrt(x);
			}
			template<size_t n, typename T>
			constexpr T SquaredDistance(const Vector<n, T>& a, const Vector<n, T>& b) {
				T x = T(0); for (size_t i = 0; i < n; i++) { x += (a.data[i] - b.data[i]) * (a.data[i] - b.data[i]); }
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

#ifdef BLUESHIFT_OPTIMIZE_SIMD
			//This is a test optimization of the Vector4f type.
			//It uses SIMD instructions for the basic arithmetic operators, but not much else.
			//Dot product and such coming soon.
			template<>
			struct Vector<4, float> {
				union {
					float data[4];
					struct { float X, Y, Z, W; };
					__m128 mm;
					Vector<3, float> XYZ;
				};
				Vector() = default;
				constexpr Vector(float x, float y, float z, float w)
					: X(x), Y(y), Z(z), W(w) {}
				constexpr explicit Vector(float s)
					: X(s), Y(s), Z(s), W(s) {}
				constexpr explicit Vector(Vector<2, float>& Other)
					: X(Other.X), Y(Other.Y), Z(0), W(0) {}
				constexpr explicit Vector(Vector<3, float>& Other)
					: X(Other.X), Y(Other.Y), Z(Other.Z), W(0) {}

				float Length() const {
					return sqrt(X * X + Y * Y + Z * Z + W * W);
				}
				float SquaredLength() const {
					return X * X + Y * Y + Z * Z + W * W;
				}

				inline float& operator[](const size_t& idx) {
					return data[idx];
				}
				inline const float& operator[](const size_t& idx) const {
					return data[idx];
				}
			};

			template<>
			inline Vector<4, float> operator+(const Vector<4, float>& a, const Vector<4, float>& b) {
				Vector<4, float> rv;
				rv.mm = _mm_add_ps(a.mm, b.mm);
				return rv;
			}
			template<>
			inline Vector<4, float> operator-(const Vector<4, float>& a, const Vector<4, float>& b) {
				Vector<4, float> rv;
				rv.mm = _mm_sub_ps(a.mm, b.mm);
				return rv;
			}
			template<>
			inline Vector<4, float> operator*(const Vector<4, float>& a, const Vector<4, float>& b) {
				Vector<4, float> rv;
				rv.mm = _mm_mul_ps(a.mm, b.mm);
				return rv;
			}
			template<>
			inline Vector<4, float> operator/(const Vector<4, float>& a, const Vector<4, float>& b) {
				Vector<4, float> rv;
				rv.mm = _mm_div_ps(a.mm, b.mm);
				return rv;
			}

			inline Vector<4, float>& operator+=(Vector<4, float>& a, const Vector<4, float>& b) {
				a.mm = _mm_add_ps(a.mm, b.mm);
				return a;
			}
			inline Vector<4, float>& operator-=(Vector<4, float>& a, const Vector<4, float>& b) {
				a.mm = _mm_sub_ps(a.mm, b.mm);
				return a;
			}
			inline Vector<4, float>& operator*=(Vector<4, float>& a, const Vector<4, float>& b) {
				a.mm = _mm_mul_ps(a.mm, b.mm);
				return a;
			}
			inline Vector<4, float>& operator/=(Vector<4, float>& a, const Vector<4, float>& b) {
				a.mm = _mm_div_ps(a.mm, b.mm);
				return a;
			}
			/*
			template<>
			float DotProduct(const Vector<4, float>& a, const Vector<4, float>& b) {
#ifdef BLUESHIFT_OPTIMIZE_SSE4_1
				return _mm_dp_ps(a.mm, b.mm, 0x7f).m128_f32[0];
#else
				const unsigned int mask_array[] = { 0xffffffff, 0xffffffff, 0xffffffff, 0 };
				const __m128 mask = _mm_load_ps(reinterpret_cast<const float*>(mask_array));
				const __m128 m = _mm_mul_ps(a.mm, b.mm);
				const __m128 s0 = _mm_and_ps(m, mask);
				const __m128 s1 = _mm_add_ps(s0, _mm_movehl_ps(s0, s0));
				const __m128 s2 = _mm_add_ss(s1, _mm_shuffle_ps(s1, s1, 1));
				return _mm_shuffle_ps(s2, s2, 0).m128_f32[0];
#endif
			}
			*/

			//TODO: implement optimized cross product and more



#endif

			typedef Vector<2, double> Vector2; typedef Vector<2, float> Vector2f;
			typedef Vector<3, double> Vector3; typedef Vector<3, float> Vector3f;
			typedef Vector<4, double> Vector4; typedef Vector<4, float> Vector4f;

			template<size_t n = 3, typename T = double>
			constexpr Vector<n, T> Right() {
				Vector<n, T> rv;
				rv.data[0] = T(1.0);
				rv.data[1] = T(0.0);
				rv.data[2] = T(0.0);
				rv.data[3] = T(0.0);
				return rv;
			}
			template<size_t n = 3, typename T = double>
			constexpr Vector<n, T> Up() {
				Vector<n, T> rv;
				rv.data[0] = T(0.0);
				rv.data[1] = T(1.0);
				rv.data[2] = T(0.0);
				rv.data[3] = T(0.0);
				return rv;
			}
			template<size_t n = 3, typename T = double>
			constexpr Vector<n, T> Forward() {
				Vector<n, T> rv;
				rv.data[0] = T(0s.0);
				rv.data[1] = T(0.0);
				rv.data[2] = T(1.0);
				rv.data[3] = T(0.0);
				return rv;
			}

			enum class CardinalAxis {
				Right,
				Up,
				Forward
			};
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

template<size_t n, typename T>
inline std::istream& operator>>(std::istream& is, Blueshift::Core::Math::Vector<n, T>& rhs) {
	for (size_t i = 0; i < n; i++) {
		is >> rhs.data[i];
	}
	return is;
}