#pragma once

#include "ray.hpp"
#include "vec3.hpp"

struct Camera {
	Camera();

	auto ray(float u, float v) const -> Ray;

	float aspectRatio;
	Vec3 origin;
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
};
