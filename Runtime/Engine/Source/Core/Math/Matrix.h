#pragma once
#include <initializer_list>
#include <inttypes.h>
#include "Core/Math/Vector.h"

namespace Blueshift {
	namespace Core {
		namespace Math {

			__declspec(align(16)) struct Matrix4 {
				union {
					float data[4 * 4];
					float m[4][4];
					Vector4 vec[4];
				};
				Matrix4() = default;
				explicit Matrix4(float s) {
					for (size_t i = 0; i < 4 * 4; i++) { data[i] = s; }
				}
				
				Matrix4(const std::initializer_list<float> list) {
					if (list.size() == 4 * 4) {
						memcpy(&data[0], list.begin(), list.size() * sizeof(float));
					}
					
				}

				Vector4& operator[](const size_t& idx) { 
					//static_assert(idx >= 0 && idx < rows, "Matrix subscript index out of bounds");
					return vec[idx];
				}
				const Vector4& operator[](const size_t& idx) const { 
					//static_assert(idx >= 0 && idx < rows, "Matrix subscript index out of bounds");
					return vec[idx]; 
				}

				constexpr float operator()(size_t i, size_t j) const {
					return data[i * 4 + j];
				}

				Matrix4 Transpose() const {
					Matrix4 out;
					for (size_t i = 0; i < 4; i++) {
						for (size_t j = 0; j < 4; j++) {
							out.data[j * 4 + i] = data[i * 4 + j];
						}
					}
					return out;
				}

				static const Matrix4 Identity;
			};

			inline Matrix4 Transpose(const Matrix4& matrix) {
				Matrix4 rv = matrix;
				_MM_TRANSPOSE4_PS(rv.vec[0].mm, rv.vec[1].mm, rv.vec[2].mm, rv.vec[3].mm);
				return rv;
			}
			inline void TransposeInplace(Matrix4& matrix) {
				_MM_TRANSPOSE4_PS(matrix.vec[0].mm, matrix.vec[1].mm, matrix.vec[2].mm, matrix.vec[3].mm);
			}

			inline Matrix4 operator +(const Matrix4& lhs, const Matrix4& rhs) {
				Matrix4 out {
					lhs[0][0] + rhs[0][0],
					lhs[0][1] + rhs[0][1],
					lhs[0][2] + rhs[0][2],
					lhs[0][3] + rhs[0][3],

					lhs[1][0] + rhs[1][0],
					lhs[1][1] + rhs[1][1],
					lhs[1][2] + rhs[1][2],
					lhs[1][3] + rhs[1][3],

					lhs[2][0] + rhs[2][0],
					lhs[2][1] + rhs[2][1],
					lhs[2][2] + rhs[2][2],
					lhs[2][3] + rhs[2][3],

					lhs[3][0] + rhs[3][0],
					lhs[3][1] + rhs[3][1],
					lhs[3][2] + rhs[3][2],
					lhs[3][3] + rhs[3][3],
				};
				
				return out;
			}

			inline Matrix4 operator -(const Matrix4& lhs, const Matrix4& rhs) {
				Matrix4 out {
					lhs[0][0] - rhs[0][0],
					lhs[0][1] - rhs[0][1],
					lhs[0][2] - rhs[0][2],
					lhs[0][3] - rhs[0][3],

					lhs[1][0] - rhs[1][0],
					lhs[1][1] - rhs[1][1],
					lhs[1][2] - rhs[1][2],
					lhs[1][3] - rhs[1][3],

					lhs[2][0] - rhs[2][0],
					lhs[2][1] - rhs[2][1],
					lhs[2][2] - rhs[2][2],
					lhs[2][3] - rhs[2][3],

					lhs[3][0] - rhs[3][0],
					lhs[3][1] - rhs[3][1],
					lhs[3][2] - rhs[3][2],
					lhs[3][3] - rhs[3][3],
				};

				return out;
			}

			inline Matrix4 operator *(const Matrix4& lhs, const Matrix4& rhs) {
#if 1
				Matrix4 out;
				Matrix4 transposed = rhs.Transpose();

				
				for (size_t i = 0; i < 4; i++) {
					for (size_t j = 0; j < 4; j++) {
						out[i][j] = DotProduct(lhs[i], transposed[j], true);
					}
				}
				return out;
#else
				Matrix4 out;
				for (size_t j = 0; j < 4; j++) {
					for (size_t i = 0; i < 4; i++) {
						//The value at i,j is just the dot product of
						//row i of a and row j of b.
						out[i][j] = 0.0f;
						for (size_t k = 0; k < 4; k++)
							out[i][j] += lhs[i][k] * rhs[k][j];
					}
				}
				return out;
#endif
			}

			inline Vector4 operator *(const Matrix4& lhs, const Vector4& rhs) {
				Vector4 out;
				Matrix4 transposed = lhs.Transpose();
				for (size_t i = 0; i < 4; i++) {
					out[i] = DotProduct(rhs, transposed.vec[i]);
				}
				return out;
			}

			inline float Determinant(const Matrix4& matrix) {
				return
					matrix(0, 3) * matrix(1, 2) * matrix(2, 1) * matrix(3, 0) - matrix(0, 2) * matrix(1, 3) * matrix(2, 1) * matrix(3, 0) -
					matrix(0, 3) * matrix(1, 1) * matrix(2, 2) * matrix(3, 0) + matrix(0, 1) * matrix(1, 3) * matrix(2, 2) * matrix(3, 0) +
					matrix(0, 2) * matrix(1, 1) * matrix(2, 3) * matrix(3, 0) - matrix(0, 1) * matrix(1, 2) * matrix(2, 3) * matrix(3, 0) -
					matrix(0, 3) * matrix(1, 2) * matrix(2, 0) * matrix(3, 1) + matrix(0, 2) * matrix(1, 3) * matrix(2, 0) * matrix(3, 1) +
					matrix(0, 3) * matrix(1, 0) * matrix(2, 2) * matrix(3, 1) - matrix(0, 0) * matrix(1, 3) * matrix(2, 2) * matrix(3, 1) -
					matrix(0, 2) * matrix(1, 0) * matrix(2, 3) * matrix(3, 1) + matrix(0, 0) * matrix(1, 2) * matrix(2, 3) * matrix(3, 1) +
					matrix(0, 3) * matrix(1, 1) * matrix(2, 0) * matrix(3, 2) - matrix(0, 1) * matrix(1, 3) * matrix(2, 0) * matrix(3, 2) -
					matrix(0, 3) * matrix(1, 0) * matrix(2, 1) * matrix(3, 2) + matrix(0, 0) * matrix(1, 3) * matrix(2, 1) * matrix(3, 2) +
					matrix(0, 1) * matrix(1, 0) * matrix(2, 3) * matrix(3, 2) - matrix(0, 0) * matrix(1, 1) * matrix(2, 3) * matrix(3, 2) -
					matrix(0, 2) * matrix(1, 1) * matrix(2, 0) * matrix(3, 3) + matrix(0, 1) * matrix(1, 2) * matrix(2, 0) * matrix(3, 3) +
					matrix(0, 2) * matrix(1, 0) * matrix(2, 1) * matrix(3, 3) - matrix(0, 0) * matrix(1, 2) * matrix(2, 1) * matrix(3, 3) -
					matrix(0, 1) * matrix(1, 0) * matrix(2, 2) * matrix(3, 3) + matrix(0, 0) * matrix(1, 1) * matrix(2, 2) * matrix(3, 3);
			}

			inline Matrix4 Inverse(const Matrix4& matrix) {
				//thanks to gluInvertMatrix, here is the fastest and most concise version of this code you're ever going to find.
				//yuck.
				const float* m = &matrix.data[0];
				Matrix4 out; float* inv = &out.data[0];

				inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
				inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
				inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
				inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
				inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
				inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
				inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
				inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
				inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
				inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
				inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
				inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
				inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
				inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
				inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
				inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

				float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

				if (det == 0.0f) {
					throw 0; //TODO actual exception?
				}
				det = 1.0f / det;

				for (size_t i = 0; i < 16; i++) {
					out.data[i] = out.data[i] * det;
				}

				return out;
			}

			inline Matrix4& Invert(Matrix4& matrix) {
				float* m = &matrix.data[0];
				float* inv = new float[16];

				inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
				inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
				inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
				inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
				inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
				inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
				inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
				inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
				inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
				inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
				inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
				inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
				inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
				inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
				inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
				inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

				float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

				if (det == 0.0f) {
					throw 0;
				}
				det = 1.0f / det;

				for (size_t i = 0; i < 16; i++) {
					m[i] = inv[i] * det;
				}

				return matrix;
			}

			inline Matrix4 ScaleMatrix(const Vector4& s) {
				Matrix4 out {
					 s.X, 0.0f, 0.0f, 0.0f,
					0.0f,  s.Y, 0.0f, 0.0f,
					0.0f, 0.0f,  s.Z, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f
				};
				return out;
			}

			inline Matrix4 TranslationMatrix(const Vector4& t) {
				Matrix4 out {
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					t.X, t.Y, t.Z, 1.0f
				};
				return out;
			}

			inline Matrix4 RotationMatrix(Vector4 axis, float angle) {
				if (LengthSquared(axis) < 1 - 1e-5 || LengthSquared(axis) > 1 + 1e-5) {
					axis = axis / Length(axis);
				}
				float y = sin(angle);
				float x = cos(angle);
				float ix = 1.0f - x;

				float ax = axis.data[0];
				float ay = axis.data[1];
				float az = axis.data[2];

				Matrix4 rv {
					ix * ax * ax + x,
					ix * ax * ay - az * y,
					ix * az * ax + ay * y,
					0.0f,

					ix * ax * ay + az * y,
					ix * ay * ay + x,
					ix * ay * az - ax * y,
					0.0f,

					ix * az * ax - ay * y,
					ix * ay * az + ax * y,
					ix * az * az + x,
					0.0f,

					0.0f, 0.0f, 0.0f, 1.0f
				};
				return rv;
			}

		}
	}
}

inline std::ostream& operator<<(std::ostream& os, const Blueshift::Core::Math::Matrix4& rhs) {
	os << "(matrix4: ";
	for (size_t i = 0; i < 16; i++) {
		os << rhs.data[i];
		if (i < 15) {
			os << ", ";
		}
	}
	os << ")";
	return os;
}