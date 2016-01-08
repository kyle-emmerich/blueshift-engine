#pragma once
#include <cmath>
#include <intrin.h>
#include "Scripting/LuaState.h"

#define BLUESHIFT_OPTIMIZE_SSE4_1

namespace Blueshift {
	namespace Core {
		namespace Math {

			const size_t Vector4PoolSize = 1024 * 10;

			/*! \class Vector4
				\brief Provides 2D, 3D, or 4D vector math in an easy to use interface.

				This class exists to provide 2D, 3D, or 4D vector math in one package. It is faster to use this class
				for 2D math than it is to use two separate floats because of SIMD optimizations. However, it can be faster
				to write your own SIMD optimizations if you have a lot of 2-component vectors to process, but it's unlikely
				that you'd find a reason for that.

				\sa Matrix4, Quaternion
			*/
			__declspec(align(16)) class Vector4 {
			public:
				/*! \brief Creates a Vector4 with uninitialized components (usually defaults to 0 on most compilers.) */
				Vector4() = default;
				/*! \brief Creates a Vector4 using a 128-bit SIMD struct. */
				inline Vector4(__m128 vec) : mm(vec) {}
				/*! \brief Creates a Vector4 from 0 to 4 floats; by default produces 0,0,0,1. */
				inline Vector4(float x, float y = 0.0f, float z = 0.0f, float w = 1.0f)
					: X(x), Y(y), Z(z), W(w) {}

				/*! \brief Wraps up the data for easy access and conversion from SIMD struct and back. */
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

				/*! \brief Retrieves or sets single value in the vector, should be 0, 1, 2, or 3 for X, Y, Z, and W respectively. */
				inline float& operator[](const size_t& idx) {
					return data[idx];
				}
				/*! \brief Retrieves a single value in the vector, should be 0, 1, 2, or 3 for X, Y, Z, and W respectively. */
				inline const float& operator[](const size_t& idx) const {
					return data[idx];
				}
			};

			/*! \relates Vector4
				\brief Adds two Vector4s and returns the result as a new Vector4.
			*/
			inline Vector4 operator+(const Vector4& a, const Vector4& b) {
				Vector4 rv;
				rv.mm = _mm_add_ps(a.mm, b.mm);
				return rv;
			}
			/*! \relates Vector4
				\brief Adds a single scalar to each component of a Vector4 and returns a new Vector4.
			*/
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

			/*! \relates Vector4
				\brief Subtracts a Vector4 from another Vector4 and returns a new Vector4.
			*/
			inline Vector4 operator-(const Vector4& a, const Vector4& b) {
				Vector4 rv;
				rv.mm = _mm_sub_ps(a.mm, b.mm);
				return rv;
			}
			/*! \relates Vector4
				\brief Subtracts a single scalar from each component of a Vector4 and returns a new Vector4.
			*/
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

			/*! \relates Vector4
				\brief Multiplies a Vector4 by another Vector4 component-wise and returns a new Vector4; sum of components is the dot product.
			*/
			inline Vector4 operator*(const Vector4& a, const Vector4& b) {
				Vector4 rv;
				rv.mm = _mm_mul_ps(a.mm, b.mm);
				return rv;
			}
			/*!	\relates Vector4
				\brief Multiplies each component of a Vector4 by a single scalar and returns a new Vector4.
			*/
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

			/*! \relates Vector4
				\brief Divides a Vector4 by another Vector4 component-wise and returns a new Vector4.
			*/
			inline Vector4 operator/(const Vector4& a, const Vector4& b) {
				Vector4 rv;
				rv.mm = _mm_div_ps(a.mm, b.mm);
				return rv;
			}
			/*! \relates Vector4
				\brief Divides each component of a Vector4 by a single scalar and returns a new Vector4.
			*/
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

			/*! \relates Vector4
				\brief Adds a Vector4 to this Vector4.
			*/
			inline Vector4& operator+=(Vector4& a, const Vector4& b) {
				a.mm = _mm_add_ps(a.mm, b.mm);
				return a;
			}
			inline Vector4& operator+=(Vector4& a, float s) {
				a.mm = _mm_add_ps(a.mm, _mm_load1_ps(&s));
				return a;
			}
			/*! \relates Vector4
			\brief Subtracts a Vector4 from this Vector4.
			*/
			inline Vector4& operator-=(Vector4& a, const Vector4& b) {
				a.mm = _mm_sub_ps(a.mm, b.mm);
				return a;
			}
			inline Vector4& operator-=(Vector4& a, float s) {
				a.mm = _mm_sub_ps(a.mm, _mm_load1_ps(&s));
				return a;
			}
			/*! \relates Vector4
			\brief Multiplies this Vector4 by another Vector4 component-wise.
			*/
			inline Vector4& operator*=(Vector4& a, const Vector4& b) {
				a.mm = _mm_mul_ps(a.mm, b.mm);
				return a;
			}
			inline Vector4& operator*=(Vector4& a, float s) {
				a.mm = _mm_mul_ps(a.mm, _mm_load1_ps(&s));
				return a;
			}
			/*! \relates Vector4
			\brief Divides this Vector4 by another Vector4 component-wise.
			*/
			inline Vector4& operator/=(Vector4& a, const Vector4& b) {
				a.mm = _mm_div_ps(a.mm, b.mm);
				return a;
			}
			inline Vector4& operator/=(Vector4& a, float s) {
				a.mm = _mm_div_ps(a.mm, _mm_load1_ps(&s));
				return a;
			}

			/*! \relates Vector4
				\brief Calculates the dot product of a Vector4 in two instructions if SSE 4.1 is enabled; otherwise in 8.

				For comparison, doing it manually would involve 4 loads, 4 multiplies, 4 adds, and 1 store; i.e. a lot more.
				Might not make a 30 FPS game into a 60 FPS game, but definitely worth it in the long run.

				\param a The first Vector4
				\param b The second Vector4
				\param with_w Should the calculation include the W component? For most applications this can be undesired.

			*/
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

			/*!	\relates Vector4
				\brief Calculates the angle between two Vector4s in radians.
			*/
			inline float AngleBetween(const Vector4& a, const Vector4& b) {
				return acos(DotProduct(a, b));
			}

			/*! \relates Vector4
				\brief Calculates the squared length of a Vector4 in 2 instructions if SSE 4.1 is enabled, otherwise in 8; avoids a square-root operation.

				\param v The Vector4
				\param with_w Should the calculation include the W component? For most applications this can be undesired.
			*/
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
			/*! \relates Vector4
				\brief Calculates the length of a Vector4.

				\param v The Vector4
				\param with_w Should the calculation include the W component? For most applications this can be undesired.
			*/
			inline float Length(const Vector4& v, bool with_w = false) {
				return sqrt(LengthSquared(v, with_w));
			}

			/*! \relates Vector4
				\brief Generates a Vector4 orthogonal to the two Vector4s provided; the cross product.

				In other words, if you had a vector pointing forward and a vector pointing up, you can find the vector pointing
				to the right with CrossProduct(forward, up).
			*/
			inline Vector4 CrossProduct(const Vector4& a, const Vector4& b) {
				Vector4 out(
					a.Y * b.Z - a.Z * b.Y,
					a.Z * b.X - a.X * b.Z,
					a.X * b.Y - a.Y * b.X,
					1.0f
				);
				return out;
			}

			/*! \relates Vector4
				\brief Calculates a point along the vector between a and b by fraction m and stores it in out.
			*/
			inline void LerpInplace(const Vector4& a, const Vector4& b, float m, Vector4& out) {
				float inv_m = 1.0f - m;
				__m128 mm_m = _mm_load1_ps(&m);
				__m128 mm_inv_m = _mm_load1_ps(&inv_m);
				out.mm = _mm_add_ps(
					_mm_mul_ps(a.mm, mm_inv_m),
					_mm_mul_ps(b.mm, mm_m)
				);
			}
			/*! \relates Vector4
				\brief Calculates a point along the vector between a and b by fraction m and returns it as a new Vector4.
			*/
			inline Vector4 Lerp(const Vector4& a, const Vector4& b, float m) {
				Vector4 out;
				LerpInplace(a, b, m, out);
				return out;
			}
		}
	}
}