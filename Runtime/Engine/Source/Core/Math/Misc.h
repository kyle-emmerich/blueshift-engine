#pragma once
#include <cmath>

namespace Blueshift {
	namespace Core {
		namespace Math {

			const double Pi = 3.14159265359;
			const double InvPi = 1.0 / Pi;
			const double DegToRad = Pi / 180.0;
			const double RadToDeg = 180.0 / Pi;
			
			template<typename T>
			constexpr T ToDegrees(T radians) {
				return radians * (T(180.0) / static_cast<T>(Pi));
			}
			template<typename T>
			constexpr T ToRadians(T degrees) {
				return degrees * (static_cast<T>(Pi) / T(180.0));
			}

			const static size_t NUM_FACTORIAL32 = 12;
			const static uint32_t FactorialLookup32[NUM_FACTORIAL32+1] = {
				1i32,
				1i32,
				2i32,
				6i32,
				24i32,
				120i32,
				720i32,
				5040i32,
				40320i32,
				362880i32,
				3628800i32,
				39916800i32,
				479001600i32
				//upper bound
			//	4294967296
			};
			const static size_t NUM_FACTORIAL64 = 20;
			const static uint64_t FactorialLookup64[NUM_FACTORIAL64 + 1] = {
				1i64,
				1i64,
				2i64,
				6i64,
				24i64,
				120i64,
				720i64,
				5040i64,
				40320i64,
				362880i64,
				3628800i64,
				39916800i64,
				479001600i64,
				6227020800i64,
				87178291200i64,
				1307674368000i64,
				20922789888000i64,
				355687428096000i64,
				6402373705728000i64,
				121645100408832000i64,
				2432902008176640000i64
				//upper bound
			//	18446744073709551616
			};
			inline uint32_t Factorial(uint32_t val) {
				if (val > NUM_FACTORIAL32) {
					throw 0; //TODO: add overflow exception
				}
				return FactorialLookup32[val];
			}
			inline uint64_t Factorial(uint64_t val) {
				if (val > NUM_FACTORIAL64) {
					throw 0; //TODO: add overflow exception
				}
				return FactorialLookup64[val];
			}

			
			template<typename T>
			constexpr T Clamp(T val, T min, T max) {
				return (val > max) ? (max : (val < min) ? min : val);
			}

			template<typename T>
			constexpr T Interpolate(T x, T y, T alpha) {
				return x * (1 - alpha) + y * alpha;
			}

			template<typename T>
			constexpr T Step(T edge, T x) {
				return x < edge ? T(0.0) : T(1.0);
			}

			template<typename T>
			constexpr T Absolute(T val) {
				return (val < 0.0) : (val * -1) : val;
			}

		}
	}
}