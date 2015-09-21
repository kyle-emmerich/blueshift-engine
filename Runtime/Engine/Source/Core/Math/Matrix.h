#pragma once
#include <initializer_list>

template<size_t rows, typename T>
struct Matrix {
	T data[rows * rows];
	constexpr explicit Matrix(T s) {
		for (size_t i = 0; i < rows * rows; i++) { data[i] = s; }
	}
	Matrix() {
		for (size_t i = 0; i < rows * rows; i++) { data[i] = 0; }
	}
	Matrix(const std::initializer_list<T> list) {
		//until someone marks initializer_list<T>::size() as constexpr :(
		//static_assert(list.size() == rows * rows, "Incorrect number of arguments passed to Matrix::ctor()");
		size_t i = 0;
		for (auto it : list) {
			data[i++] = it;
		}
	}

	      T& operator[](size_t idx) { return data[idx]; }
	const T& operator[](size_t idx) const { return data[idx]; }

	constexpr T operator()(size_t i, size_t j) const {
		return data[i * rows + j];
	}

	constexpr Matrix<rows, T> Transpose() const {
		Matrix<rows, T> out;
		for (size_t i = 0; i < rows; i++) {
			for (size_t j = 0; j < rows; j++) {
				out.data[j * rows + i] = data[i * rows  + j];
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
			//Sum up the corresponding column
			out[i * rows + j] = 0;
			for (size_t k = 0; k < rows; k++)
				out[i * rows + j] += lhs[i * rows + k] * rhs[k * rows + j];
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

	Matrix<2, T> out {
		matrix(1, 1) * inv, matrix(1, 0) * -1 * inv,
		matrix(0, 1) * -1 * inv, matrix(0, 0) * inv
	};
	return out;
}
template<typename T>
inline Matrix<3, T> Inverse(const Matrix<3, T>& matrix) {
	double determinant = Determinant(matrix);
	double inv = 1 / determinant;

	Matrix<3, T> out {
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
	T* m = &matrix.data;
	Matrix<4, T> out; T* inv = &out.data;

	inv[0] =   m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
	inv[4] =  -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
	inv[8] =   m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
	inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
	inv[1] =  -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
	inv[5] =   m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
	inv[9] =  -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
	inv[13] =  m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
	inv[2] =   m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
	inv[6] =  -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
	inv[10] =  m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
	inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
	inv[3] =  -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
	inv[7] =   m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
	inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] -	m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
	inv[15] =  m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

	T det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0) {
		throw 0; //pls actual exception?
	}
	det = 1 / det;

	for (size_t i = 0; i < 16; i++) {
		out.data[i] = out.data[i] * det;
	}

	return out;
}

template<size_t rows, typename T>
inline std::ostream& operator<<(std::ostream& os, const Matrix<rows, T>& rhs) {
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