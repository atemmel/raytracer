#pragma once

#include "vec3.hpp"

#include <cstdlib>

struct Box {
	Vec3 min;
	Vec3 max;
	size_t material;
};
