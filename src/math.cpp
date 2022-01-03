#include "math.hpp"

#include <random>

auto degreesToRadians(float theta) -> float {
	return theta * Pi / 180.f;
}

auto radiansToDegrees(float theta) -> float {
	return theta * 180.f / Pi;
}

auto randomFloat(float min, float max) -> float {
	std::uniform_real_distribution<float> distribution(min, max);
	static thread_local std::mt19937 generator(std::random_device{}());
	return distribution(generator);
}

auto randomUnitDisk(float &x, float &y) -> void {
	for(;;) {
		x = randomFloat();
		y = randomFloat();
		if(std::sqrt(x * x + y * y) <= 1.f) {
			return;
		}
	}
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
