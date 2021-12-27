#include "math.hpp"

constexpr auto degreesToRadians(float theta) -> float {
	return theta * Pi / 180.f;
}

constexpr auto radiansToDegrees(float theta) -> float {
	return theta * 180.f / Pi;
}
