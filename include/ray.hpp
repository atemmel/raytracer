#pragma once

#include "vec3.hpp"

struct Ray {
	Ray() = default;
	Ray(Vec3 origin, Vec3 direction);

	auto at(float step) const -> Vec3;

	Vec3 origin;
	Vec3 direction;
};
