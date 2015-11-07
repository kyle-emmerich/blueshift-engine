#pragma once
#include <initializer_list>
#include <inttypes.h>
#include "Core/Math/Vector.h"

namespace Blueshift {
	namespace Core {
		namespace Math {

			template<size_t rows, typename T>
			struct Matrix {
				union {
					T data[rows * rows];
					T m[rows][rows];
					Vector<rows, T> vec[rows];
				};
				constexpr explicit Matrix(T s) {
					for (size_t i = 0; i < rows * rows; i++) { data[i] = s; }
				}
				Matrix() {
					for (size_t i = 0; i < rows * rows; i++) { data[i] = 0; }
				}
				Matrix(const std::initializer_list<T> list) {
					if (list.size() == rows * rows) {
						size_t i = 0;
						for (auto it : list) {
							data[i++] = it;
						}
					} else {
						if (list.size() == 9 && rows == 4) {
							//given 3x3 data for a 4x4 matrix, we can handle this as a fun special case
							//however, initializer_list provides no [] operator, so we REALLY get creative...
							const T* x = list.begin();

							data[0] = *x++;
							data[1] = *x++;
							data[2] = *x++;
							data[3] = 0.0;

							data[4] = *x++;
							data[5] = *x++;
							data[6] = *x++;
							data[7] = 0.0;

							data[8] = *x++;
							data[9] = *x++;
							data[10] = *x++;
							data[11] = 0.0;

							data[12] = 0.0;
							data[13] = 0.0;
							data[14] = 0.0;
							data[15] = 1.0;
						}
					}
					
				}

				Vector<rows, T>& operator[](const size_t& idx) { 
					//static_assert(idx >= 0 && idx < rows, "Matrix subscript index out of bounds");
					return vec[idx];
				}
				const Vector<rows, T>& operator[](const size_t& idx) const { 
					//static_assert(idx >= 0 && idx < rows, "Matrix subscript index out of bounds");
					return vec[idx]; 
				}

				constexpr T operator()(size_t i, size_t j) const {
					return data[i * rows + j];
				}

				constexpr Matrix<rows, T> Transpose() const {
					Matrix<rows, T> out;
					for (size_t i = 0; i < rows; i++) {
						for (size_t j = 0; j < rows; j++) {
							out.data[j * rows + i] = data[i * rows + j];
						}
					}
					return out;
				}
			};

			template<size_t rows, typename T>
			Matrix<rows, T> operator +(const Matrix<rows, T>& lhs, const Matrix<rows, T>& rhs) {
				Matrix<rows, T> out;
				for (size_t i = 0; i < rows * rows; i++) {
					out.data[i] = lhs.data[i] + rhs.data[i];
				}
				return out;
			}

			template<size_t rows, typename T>
			Matrix<rows, T> operator -(const Matrix<rows, T>& lhs, const Matrix<rows, T>& rhs) {
				Matrix<rows, T> out;
				for (size_t i = 0; i < rows; i++) {
					out.data[i] = lhs.data[i] - rhs.data[i];
				}
				return out;
			}


			template<size_t rows, typename T>
			inline Matrix<rows, T> operator *(const Matrix<rows, T>& lhs, const Matrix<rows, T>& rhs) {
				Matrix<rows, T> out;
				for (size_t i = 0; i < rows; i++) {
					for (size_t j = 0; j < rows; j++) {
						//The value at i,j is just the dot product of
						//row i of a and row j of b.
						out[i][j] = 0;
						for (size_t k = 0; k < rows; k++)
							out[i][j] += lhs[i][k] * rhs[k][j];
					}
				}
				return out;
			}

			typedef Matrix<2, double> Matrix2; typedef Matrix<2, float> Matrix2f;
			typedef Matrix<3, double> Matrix3; typedef Matrix<3, float> Matrix3f;
			typedef Matrix<4, double> Matrix4; typedef Matrix<4, float> Matrix4f;

			template <size_t rows, typename T>
			inline T Determinant(const Matrix<rows, T>& matrix) {
				throw 0;
			}

			template <typename T>
			inline T Determinant(const Matrix<1, T>& matrix) {
				return matrix(0, 0);
			}

			template <typename T>
			inline T Determinant(const Matrix<2, T>& matrix) {
				return matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0);
			}
			template <typename T>
			inline T Determinant(const Matrix<3, T>& matrix) {
				return matrix(0, 0) * matrix(1, 1) * matrix(2, 2) +
					matrix(1, 0) * matrix(2, 1) * matrix(0, 2) +
					matrix(2, 0) * matrix(0, 1) * matrix(1, 2) -
					matrix(2, 0) * matrix(1, 1) * matrix(0, 2) -
					matrix(1, 0) * matrix(0, 1) * matrix(2, 2) -
					matrix(0, 0) * matrix(2, 1) * matrix(1, 2);
			}
			template<typename T>
			inline T Determinant(const Matrix<4, T>& matrix) {
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

			template<typename T>
			inline Matrix<1, T> Inverse(const Matrix<1, T>& matrix) {
				return Matrix<1, T>(1 / matrix.data[0]);
			}
			template<typename T>
			inline Matrix<2, T> Inverse(const Matrix<2, T>& matrix) {
				double determinant = Determinant(matrix);
				double inv = 1 / determinant;

				Matrix<2, T> out{
					matrix(1, 1) * inv, matrix(1, 0) * -1 * inv,
					matrix(0, 1) * -1 * inv, matrix(0, 0) * inv
				};
				return out;
			}
			template<typename T>
			inline Matrix<3, T> Inverse(const Matrix<3, T>& matrix) {
				double determinant = Determinant(matrix);
				double inv = 1 / determinant;

				Matrix<3, T> out{
					(matrix(1, 1) * matrix(2, 2) - matrix(2, 1) * matrix(1, 2)) * inv,
					(matrix(0, 2) * matrix(2, 1) - matrix(0, 1) * matrix(2, 2)) * inv,
					(matrix(0, 1) * matrix(1, 2) - matrix(0, 2) * matrix(1, 1)) * inv,
					(matrix(1, 2) * matrix(2, 0) - matrix(1, 0) * matrix(2, 2)) * inv,
					(matrix(0, 0) * matrix(2, 2) - matrix(0, 2) * matrix(2, 0)) * inv,
					(matrix(1, 0) * matrix(0, 2) - matrix(0, 0) * matrix(1, 2)) * inv,
					(matrix(1, 0) * matrix(2, 1) - matrix(2, 0) * matrix(1, 1)) * inv,
					(matrix(2, 0) * matrix(0, 1) - matrix(0, 0) * matrix(2, 1)) * inv,
					(matrix(0, 0) * matrix(1, 1) - matrix(1, 0) * matrix(0, 1)) * inv
				};
				return out;
			}
			template<typename T>
			inline Matrix<4, T> Inverse(const Matrix<4, T>& matrix) {
				//thanks to gluInvertMatrix, here is the fastest and most concise version of this code you're ever going to find.
				//yuck.
				const T* m = &matrix.data[0];
				Matrix<4, T> out; T* inv = &out.data[0];

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

				T det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

				if (det == 0) {
					throw 0; //TODO actual exception?
				}
				det = 1 / det;

				for (size_t i = 0; i < 16; i++) {
					out.data[i] = out.data[i] * det;
				}

				return out;
			}

			template<typename T = double>
			inline Matrix<4, T> ScaleMatrix(Vector<3, T> t) {
				return Matrix<4, T> {
					t.data[0], 0.0, 0.0, 0.0,
					0.0, t.data[1], 0.0, 0.0,
					0.0, 0.0, t.data[2], 0.0,
					0.0, 0.0, 0.0, 1.0
				};
			}

			template<typename T = double>
			inline Matrix<4, T> TranslationMatrix(Vector<3, T> t) {
				return Matrix<4, T> {
					1.0, 0.0, 0.0, 0.0,
					0.0, 1.0, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					t.data[0], t.data[1], t.data[2], 1.0
				};
			}

			template<size_t n, typename T = double>
			inline Matrix<n, T> RotationMatrix(Vector<3, T> axis, T angle) {
				if (axis.SquaredLength() < 1 - 1e-5 || axis.SquaredLength() > 1 + 1e-5) {
					axis /= axis.Length();
				}
				T y = sin(angle);
				T x = cos(angle);
				T ix = T(1.0) - x;

				T ax = axis.data[0];
				T ay = axis.data[1];
				T az = axis.data[2];

				return Matrix<n, T> {
					ix * ax * ax + x,
					ix * ax * ay - az * y,
					ix * az * ax + ay * y,

					ix * ax * ay + az * y,
					ix * ay * ay + x,
					ix * ay * az - ax * y,

					ix * az * ax - ay * y,
					ix * ay * az + ax * y,
					ix * az * az + x
				};
			}

			inline Matrix<4, float> Downgrade(Matrix<4, double>& m) {
				return Matrix<4, float> {
					float(m.data[0]), float(m.data[1]), float(m.data[2]), float(m.data[3]),
					float(m.data[4]), float(m.data[5]), float(m.data[6]), float(m.data[7]),
					float(m.data[8]), float(m.data[9]), float(m.data[10]), float(m.data[11]),
					float(m.data[12]), float(m.data[13]), float(m.data[14]), float(m.data[15])
				};
			}


			static const Matrix3 IdentityMatrix3 {
				1.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0
			};

			static const Matrix4 IdentityMatrix4 {
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0
			};



		}
	}
}

template<size_t rows, typename T>
inline std::ostream& operator<<(std::ostream& os, const Blueshift::Core::Math::Matrix<rows, T>& rhs) {
	os << "(" << rows << "x" << rows << " matrix: ";
	for (size_t i = 0; i < rows * rows; i++) {
		os << rhs.data[i];
		if (i < (rows * rows) - 1) {
			os << ", ";
		}
	}
	os << ")";
	return os;
}