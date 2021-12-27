#include "world.hpp"

auto World::add(Sphere sphere) -> void {
	spheres.push_back(sphere);
}
