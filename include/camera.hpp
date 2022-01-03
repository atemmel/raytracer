#pragma once

#include "ray.hpp"
#include "vec3.hpp"

struct Camera {
	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 up, float verticalFov, float aspectRatio, float aperture, float focusDistance);

	auto ray(float u, float v) const -> Ray;

	float aspectRatio;
	Vec3 origin;
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	float radius;
};
