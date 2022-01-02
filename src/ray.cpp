#include "ray.hpp"

#include <array>
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

auto Ray::hit(const Box& box, float minStep, float maxStep, RayHitData& data) const -> bool {
	const Vec3 delta = box.max - box.min;

	enum Which {
		x, y, z,
	};

	Which one = x;
	Which two = x;

	if(direction.x == 0.f && (origin.x < -delta.x || origin.x > delta.x)) {
		return false;
	}

	float near = (box.min.x - origin.x) / direction.x;
	float far = (box.max.x - origin.x) / direction.x;

	if(near > far) {
		std::swap(near, far);
	}

	if(far < 0.f) {
		return false;
	}

	if(direction.y == 0.f && (origin.y < -delta.y || origin.y > delta.y)) {
		return false;
	}

	float y0 = (box.min.y - origin.y) / direction.y;
	float y1 = (box.max.y - origin.y) / direction.y;

	if(y0 > y1) {
		std::swap(y0, y1);
	}

	if(y0 > near) {
		near = y0;
		one = y;
	}

	if(y1 < far) {
		far = y1;
		two = y;
	}

	if(near > far) {
		return false;
	}

	if(far < 0.f) {
		return false;
	}

	if(direction.z == 0.f && (origin.z < -delta.z || origin.z > delta.z)) {
		return false;
	}

	float z0 = (box.min.z - origin.z) / direction.z;
	float z1 = (box.max.z - origin.z) / direction.z;

	if(z0 > z1) {
		std::swap(z0, z1);
	}

	if(z0 > near) {
		near = z0;
		one = z;
	}

	if(z1 < far) {
		far = z1;
		two = z;
	}

	if(near > far) {
		return false;
	}

	if(far < 0.f) {
		return false;
	}

	constexpr static std::array<Vec3, 3> allNormals = {
		Vec3{1.f, 0.f, 0.f},
		Vec3{0.f, 1.f, 0.f},
		Vec3{0.f, 0.f, 1.f},
	};

	if(near <= 0.f) {
		data.step = far;
		data.normal = allNormals[two];
	} else {
		data.step = near;
		data.normal = allNormals[one];
	}

	if(data.step > maxStep || data.step < minStep) {
		return false;
	}

	if(data.normal.dot(direction) > 0.f) {
		data.normal = -data.normal;
	}
	data.normal.unit();
	data.point = at(data.step);
	return true;
}

auto Ray::hit(const World& world, float minStep, float maxStep, RayHitData& data) const -> bool {
	auto tempData = RayHitData{};
	bool didHit = false;
	float closestHit = maxStep;
	float boxClosestHit = maxStep;

	for(auto sphere : world.spheres) {
		if(hit(sphere, minStep, closestHit, tempData)) {
			didHit = true;
			closestHit = tempData.step;
			data = tempData;
		}
	}

	for(const auto& box : world.boxes) {
		if(hit(box, minStep, closestHit, tempData)) {
			didHit = true;
			closestHit = tempData.step;
			data = tempData;
		}
	}

	return didHit;
}
