#pragma once

#include "box.hpp"
#include "sphere.hpp"

#include <vector>

struct World {
	auto add(const Box& box) -> void;
	auto add(Sphere sphere) -> void;

	std::vector<Box> boxes;
	std::vector<Sphere> spheres;
};
