#pragma once
#include <inttypes.h>
#include "Core/Math/Vector4.h"

namespace Blueshift {
	namespace Core {
		namespace Math {

			struct ColorRGBA8 {
				union {
					uint8_t data[4];
					struct {
						uint8_t R;
						uint8_t G;
						uint8_t B;
						uint8_t A;
					};
					
					uint32_t RGBA;
				};
				inline explicit ColorRGBA8(uint32_t RGBA)
					: RGBA(RGBA) { }
				inline ColorRGBA8(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha = 255)
					: R(Red), G(Green), B(Blue), A(Alpha) { }
				inline ColorRGBA8(float Red, float Green, float Blue, float Alpha = 1.0f) {
					R = static_cast<uint8_t>(Red * 255.0f);
					G = static_cast<uint8_t>(Green * 255.0f);
					B = static_cast<uint8_t>(Blue * 255.0f);
					A = static_cast<uint8_t>(Alpha * 255.0f);
				}
				ColorRGBA8() = default;
				
				inline float Red() const {
					return static_cast<float>(R) / 255.0f;
				}
				inline void Red(float r) {
					R = static_cast<uint8_t>(r * 255.0f);
				}

				inline float Green() const {
					return static_cast<float>(G) / 255.0f;
				}
				inline void Green(float g) {
					G = static_cast<uint8_t>(g * 255.0f);
				}

				inline float Blue() const {
					return static_cast<float>(B) / 255.0f;
				}
				inline void Blue(float b) {
					B = static_cast<uint8_t>(b * 255.0f);
				}

				inline float Alpha() const {
					return static_cast<float>(A) / 255.0f;
				}
				inline void Alpha(float a) {
					A = static_cast<uint8_t>(a * 255.0f);
				}

				inline Vector4 ToFloatingPoint() const {
					return Vector4(Red(), Green(), Blue(), Alpha());
				}
				inline void FromFloatingPoint(const Vector4& rgba) {
					R = static_cast<uint8_t>(rgba.X * 255.0f);
					G = static_cast<uint8_t>(rgba.Y * 255.0f);
					B = static_cast<uint8_t>(rgba.Z * 255.0f);
					A = static_cast<uint8_t>(rgba.W * 255.0f);
				}
			};

		}
	}
}