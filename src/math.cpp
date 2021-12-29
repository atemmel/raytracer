#include "math.hpp"

#include <random>

constexpr auto degreesToRadians(float theta) -> float {
	return theta * Pi / 180.f;
}

constexpr auto radiansToDegrees(float theta) -> float {
	return theta * 180.f / Pi;
}

auto randomFloat(float min, float max) -> float {
	static thread_local std::uniform_real_distribution<float> distribution(min, max);
	static thread_local std::mt19937 generator(std::random_device{}());
	return distribution(generator);
}

auto clamp(float a, float min, float max) -> float {
	if(a < min) {
		return min;
	}
	if(a > max) {
		return max;
	}
	return a;
}
