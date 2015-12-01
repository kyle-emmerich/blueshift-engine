#pragma once
#include <cmath>
#include <intrin.h>
#include "Scripting/LuaState.h"

#define BLUESHIFT_OPTIMIZE_SSE4_1

namespace Blueshift {
	namespace Core {
		namespace Math {

			const size_t Vector4PoolSize = 1024 * 10;

			__declspec(align(16)) class Vector4 {
			public:
				Vector4() = default;
				inline Vector4(__m128 vec) : mm(vec) {}
				inline Vector4(float x, float y = 0.0f, float z = 0.0f, float w = 1.0f)
					: X(x), Y(y), Z(z), W(w) {}

				union {
					struct {
						float X;
						float Y;
						float Z;
						float W;
					};
					float data[4];
					__m128 mm;
					
				};

				inline float& operator[](const size_t& idx) {
					return data[idx];
				}
				inline const float& operator[](const size_t& idx) const {
					return data[idx];
				}
			};

			inline Vector4 operator+(const Vector4& a, const Vector4& b) {
				Vector4 rv;
				rv.mm = _mm_add_ps(a.mm, b.mm);
				return rv;
			}
			inline Vector4 operator+(const Vector4& a, float s) {
				Vector4 rv;
				rv.mm = _mm_add_ps(a.mm, _mm_load1_ps(&s));
				return rv;
			}
			inline void AddInplace(const Vector4& a, const Vector4& b, Vector4& rv) {
				rv.mm = _mm_add_ps(a.mm, b.mm);
			}
			inline void AddInplace(const Vector4& a, float s, Vector4& rv) {
				rv.mm = _mm_add_ps(a.mm, _mm_load1_ps(&s));
			}

			inline Vector4 operator-(const Vector4& a, const Vector4& b) {
				Vector4 rv;
				rv.mm = _mm_sub_ps(a.mm, b.mm);
				return rv;
			}
			inline Vector4 operator-(const Vector4& a, float s) {
				Vector4 rv;
				rv.mm = _mm_sub_ps(a.mm, _mm_load1_ps(&s));
				return rv;
			}
			inline void SubInplace(const Vector4& a, const Vector4& b, Vector4& rv) {
				rv.mm = _mm_sub_ps(a.mm, b.mm);
			}
			inline void SubInplace(const Vector4& a, float s, Vector4& rv) {
				rv.mm = _mm_sub_ps(a.mm, _mm_load1_ps(&s));
			}

			inline Vector4 operator*(const Vector4& a, const Vector4& b) {
				Vector4 rv;
				rv.mm = _mm_mul_ps(a.mm, b.mm);
				return rv;
			}
			inline Vector4 operator*(const Vector4& a, float s) {
				Vector4 rv;
				rv.mm = _mm_mul_ps(a.mm, _mm_load1_ps(&s));
				return rv;
			}
			inline void MulInplace(const Vector4& a, const Vector4& b, Vector4& rv) {
				rv.mm = _mm_mul_ps(a.mm, b.mm);
			}
			inline void MulInplace(const Vector4& a, float s, Vector4& rv) {
				rv.mm = _mm_mul_ps(a.mm, _mm_load1_ps(&s));
			}

			inline Vector4 operator/(const Vector4& a, const Vector4& b) {
				Vector4 rv;
				rv.mm = _mm_div_ps(a.mm, b.mm);
				return rv;
			}
			inline Vector4 operator/(const Vector4& a, float s) {
				Vector4 rv;
				rv.mm = _mm_div_ps(a.mm, _mm_load1_ps(&s));
				return rv;
			}
			inline void DivInplace(const Vector4& a, const Vector4& b, Vector4& rv) {
				rv.mm = _mm_div_ps(a.mm, b.mm);
			}
			inline void DivInplace(const Vector4& a, float s, Vector4& rv) {
				rv.mm = _mm_div_ps(a.mm, _mm_load1_ps(&s));
			}

			inline Vector4& operator+=(Vector4& a, const Vector4& b) {
				a.mm = _mm_add_ps(a.mm, b.mm);
				return a;
			}
			inline Vector4& operator+=(Vector4& a, float s) {
				a.mm = _mm_add_ps(a.mm, _mm_load1_ps(&s));
				return a;
			}
			inline Vector4& operator-=(Vector4& a, const Vector4& b) {
				a.mm = _mm_sub_ps(a.mm, b.mm);
				return a;
			}
			inline Vector4& operator-=(Vector4& a, float s) {
				a.mm = _mm_sub_ps(a.mm, _mm_load1_ps(&s));
				return a;
			}
			inline Vector4& operator*=(Vector4& a, const Vector4& b) {
				a.mm = _mm_mul_ps(a.mm, b.mm);
				return a;
			}
			inline Vector4& operator*=(Vector4& a, float s) {
				a.mm = _mm_mul_ps(a.mm, _mm_load1_ps(&s));
				return a;
			}
			inline Vector4& operator/=(Vector4& a, const Vector4& b) {
				a.mm = _mm_div_ps(a.mm, b.mm);
				return a;
			}
			inline Vector4& operator/=(Vector4& a, float s) {
				a.mm = _mm_div_ps(a.mm, _mm_load1_ps(&s));
				return a;
			}

			inline float DotProduct(const Vector4& a, const Vector4& b, bool with_w = false) {
#ifdef BLUESHIFT_OPTIMIZE_SSE4_1
				return with_w ? _mm_dp_ps(a.mm, b.mm, 0xFF).m128_f32[0] :
								_mm_dp_ps(a.mm, b.mm, 0x7F).m128_f32[0];
#else
				__declspec(align(16)) const unsigned int mask_array[] = { 0xFFFFFFFFFF, 0xFFFFFFFFFF, 0xFFFFFFFFFF, with_w ? 0xFFFFFFFFFF : 0 };
				const __m128 mask = _mm_load_ps(reinterpret_cast<const float*>(mask_array));
				const __m128 m = _mm_mul_ps(a.mm, b.mm);
				const __m128 s0 = _mm_and_ps(m, mask);
				const __m128 s1 = _mm_add_ps(s0, _mm_movehl_ps(s0, s0));
				const __m128 s2 = _mm_add_ss(s1, _mm_shuffle_ps(s1, s1, 1));
				return _mm_shuffle_ps(s2, s2, 0).m128_f32[0];
#endif
			}

			inline float AngleBetween(const Vector4& a, const Vector4& b) {
				return acos(DotProduct(a, b));
			}

			inline float LengthSquared(const Vector4& v, bool with_w = false) {
#ifdef BLUESHIFT_OPTIMIZE_SSE4_1
				return with_w ? _mm_dp_ps(v.mm, v.mm, 0xFF).m128_f32[0] :
								_mm_dp_ps(v.mm, v.mm, 0x7F).m128_f32[0];
#else
				__declspec(align(16)) const unsigned int mask_array[] = { 0xFFFFFFFFFF, 0xFFFFFFFFFF, 0xFFFFFFFFFF, with_w ? 0xFFFFFFFFFF : 0 };
				const __m128 mask = _mm_load_ps(reinterpret_cast<const float*>(mask_array));
				const __m128 m = _mm_mul_ps(v.mm, v.mm);
				const __m128 s0 = _mm_and_ps(m, mask);
				const __m128 s1 = _mm_add_ps(s0, _mm_movehl_ps(s0, s0));
				const __m128 s2 = _mm_add_ss(s1, _mm_shuffle_ps(s1, s1, 1));
				return _mm_shuffle_ps(s2, s2, 0).m128_f32[0];
#endif
			}
			inline float Length(const Vector4& v, bool with_w = false) {
				return sqrt(LengthSquared(v, with_w));
			}

			inline Vector4 CrossProduct(const Vector4& a, const Vector4& b) {
				Vector4 out(
					a.Y * b.Z - a.Z * b.Y,
					a.Z * b.X - a.X * b.Z,
					a.X * b.Y - a.Y * b.X,
					1.0f
				);
				return out;
			}

			inline void LerpInplace(const Vector4& a, const Vector4& b, float m, Vector4& out) {
				float inv_m = 1.0f - m;
				__m128 mm_m = _mm_load1_ps(&m);
				__m128 mm_inv_m = _mm_load1_ps(&inv_m);
				out.mm = _mm_add_ps(
					_mm_mul_ps(a.mm, mm_inv_m),
					_mm_mul_ps(b.mm, mm_m)
				);
			}
			inline Vector4 Lerp(const Vector4& a, const Vector4& b, float m) {
				Vector4 out;
				LerpInplace(a, b, m, out);
				return out;
			}

			/*
			class Vector4Pool {
			private:
				static Vector4Pool* instance;
				Vector4* pool;
				Vector4* first;
			public:
				static Vector4Pool& Get() {
					if (instance == nullptr) {
						instance = new Vector4Pool;
					}
					return *instance;
				}
				inline Vector4Pool() {
					pool = new Vector4[Vector4PoolSize];
					for (size_t i = 0; i < Vector4PoolSize - 1; i++) {
						pool[i].next = &pool[i + 1];
					}
					first = &pool[0];
				}

				inline Vector4& Create(float X, float Y, float Z, float W) {
					Vector4& vec = *first;
					first = vec.next;
					vec.X = X;
					vec.Y = Y;
					vec.Z = Z;
					vec.W = W;
					return vec;
				}
				inline void Destroy(Vector4& vec) {
					vec.next = first;
					first = &vec;
				}

			};

			static Vector4& NewVector4(float X = 0.0f, float Y = 0.0f, float Z = 0.0f, float W = 0.0f) {
				return Vector4Pool::Get().Create(X, Y, Z, W);
			}
			static void Destroy(Vector4& vec) {
				Vector4Pool::Get().Destroy(vec);
			}*/
		}
	}
}