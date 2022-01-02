#include "world.hpp"

auto World::add(const Box& box) -> void {
	boxes.push_back(box);
}

auto World::add(Sphere sphere) -> void {
	spheres.push_back(sphere);
}
