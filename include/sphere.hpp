#pragma once

#include "vec3.hpp"

#include <cstdlib>

struct Sphere {
	Vec3 origin;
	float radius;
	size_t material;
};
