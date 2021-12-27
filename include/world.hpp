#pragma once

#include "sphere.hpp"

#include <vector>

struct World {
	auto add(Sphere sphere) -> void;

	std::vector<Sphere> spheres;
};
