#include "ray.hpp"

#include <cmath>

auto Ray::at(float step) const -> Vec3 {
	return origin + (step * direction);
}

auto Ray::hit(Sphere sphere, float minStep, float maxStep, RayHitData& data) const -> bool {
	auto oc = origin - sphere.origin;
	auto a = direction.squaredNorm();
	auto halfB = oc.dot(direction);
	auto c = oc.squaredNorm() - sphere.radius * sphere.radius;
	auto discriminant = halfB * halfB - a * c;
	if(discriminant < 0.f) {
		return false;
	}
	auto sqrtDiscriminant = std::sqrt(discriminant);

	// same as before, but now with a range check
	auto root = (-halfB - sqrtDiscriminant) / a;
	if(root < minStep || maxStep < root) {
		root = (-halfB + sqrtDiscriminant) / a;
		if(root < minStep || maxStep < root) {
			return false;
		}
	}

	// collect relevant data
	data.step = root;
	data.point = at(root);
	data.normal = (data.point - sphere.origin) / sphere.radius;
	return true;
}

auto Ray::hit(const World& world, float minStep, float maxStep, RayHitData& data) const -> bool {
	auto tempData = RayHitData{};
	bool didHit = false;
	float closestHit = maxStep;

	// extend for other shapes
	for(auto sphere : world.spheres) {
		if(hit(sphere, minStep, closestHit, tempData)) {
			didHit = true;
			closestHit = data.step;
			data = tempData;
		}
	}

	return didHit;
}
