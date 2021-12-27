#pragma once

#include "sphere.hpp"
#include "vec3.hpp"
#include "world.hpp"

struct RayHitData {
	Vec3 point;
	Vec3 normal;
	float step;
};

struct Ray {
	auto at(float step) const -> Vec3;

	auto hit(Sphere sphere, float minStep, float maxStep, RayHitData& data) const -> bool;

	auto hit(const World& world, float minStep, float maxStep, RayHitData& data) const -> bool;

	Vec3 origin;
	Vec3 direction;
};
