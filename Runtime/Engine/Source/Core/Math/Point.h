#pragma once
#include <cmath>

template<typename T, size_t Dimensions = 3>
class Point {
private:
	T data[Dimensions];

public:
	constexpr Point() {
		for (size_t i = 0; i < Dimensions; i++) {
			data[i] = 0.0;
		}
	}
};