#pragma once
#include <cmath>
/*
template<size_t Components, typename T>
class Vector {
public:
	T data[Components];

public:
	constexpr Vector(T x, T y, T z)
		: X(x), Y(y), Z(z) {}
	constexpr explicit Vector(T scalar)
		: X(scalar), Y(scalar), Z(scalar) {}

	inline Vector3 operator+ (const Vector3& other) const {
		return Vector3(X + other.X, Y + other.Y, Z + other.Z);
	}
	inline Vector3 operator+=(const Vector3& other) {
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		return *this;
	}
	inline Vector3 operator+ (T scalar) const {
		return Vector3(X + scalar, Y + scalar, Z + scalar);
	}
	inline Vector3 operator+=(T scalar) {
		X += scalar;
		Y += scalar;
		Z += scalar;
		return *this;
	}

	inline Vector3 operator- (const Vector3& other) const {
		return Vector3(X - other.X, Y - other.Y, Z - other.Z);
	}
	inline Vector3 operator-=(const Vector3& other) {
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		return *this;
	}
	inline Vector3 operator- (T scalar) const {
		return Vector3(X - scalar, Y - scalar, Z - scalar);
	}
	inline Vector3 operator-=(T scalar) {
		X -= scalar;
		Y -= scalar;
		Z -= scalar;
		return *this;
	}

	inline Vector3 operator* (const Vector3& other) const {
		return Vector3(X * other.X, other.Y * other.Y, Z * other.Z);
	}
	inline Vector3 operator*=(const Vector3& other) {
		X *= other.X;
		Y *= other.Y;
		Z *= other.Z;
		return *this;
	}
	inline Vector3 operator* (T scalar) const {
		return Vector3(X * scalar, Y * scalar, Z * scalar);
	}
	inline Vector3 operator*=(T scalar) {
		X *= scalar;
		Y *= scalar;
		Z *= scalar;
		return *this;
	}

	inline Vector3 operator/ (const Vector3& other) const {
		return Vector3(X / other.X, other.Y / other.Y, Z / other.Z);
	}
	inline Vector3 operator/=(const Vector3& other) {
		X /= other.X;
		Y /= other.Y;
		Z /= other.Z;
		return *this;
	}
	inline Vector3 operator/ (T scalar) const {
		return Vector3(X / scalar, Y / scalar, Z / scalar);
	}
	inline Vector3 operator/=(T scalar) {
		X /= scalar;
		Y /= scalar;
		Z /= scalar;
		return *this;
	}

	inline T SquaredMagnitude() const {
		return X * X + Y * Y + Z * Z;
	}
	inline T Magnitude() const {
		return sqrt(X * X + Y * Y + Z * Z);
	}

	inline static bool IsWithin(Vector3& lhs, Vector3& rhs, double distance) {
		return ((rhs.X - lhs.X) * (rhs.X - lhs.X) +
				(rhs.Y - lhs.Y) * (rhs.Y - lhs.Y) +
				(rhs.Z - lhs.Z) * (rhs.Z - lhs.Z)) < distance;
	}

	inline static T DotProduct(const Vector3& lhs, const Vector3& rhs) {
		return (lhs.X * rhs.X) + (lhs.Y * rhs.Y) + (lhs.Z * rhs.Z);
	}
	inline static T AngleBetween(const Vector3& lhs, const Vector3& rhs) {
		return acos((lhs.X * rhs.X) + (lhs.Y * rhs.Y) + (lhs.Z * rhs.Z));
	}
	inline static Vector3 CrossProduct(Vector3& lhs, Vector3& rhs) {
		return Vector3(
			lhs.Y * rhs.Z - lhs.Z * rhs.Y,
			lhs.Z * rhs.X - lhs.X * rhs.Z,
			lhs.X * rhs.Y - lhs.Y * rhs.X
		);
	}

	inline static Vector3<float> Reduce(Vector3<double>& v) {
		return Vector3<float>(
			static_cast<float>(v.X),
			static_cast<float>(v.Y),
			static_cast<float>(v.Z)
			);
	}
	inline static Vector3<double> Expand(Vector3<float>& v) {
		return Vector3<double>(
			static_cast<double>(v.X),
			static_cast<double>(v.Y),
			static_cast<double>(v.Z)
			);
	}
};



typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;*/