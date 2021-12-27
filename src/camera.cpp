#include "camera.hpp"

Camera::Camera() {
	aspectRatio = 16.f / 9.f;
	auto viewportHeight = 2.f;
	auto viewportWidth = aspectRatio * viewportHeight;
	auto focalLength = 1.f;
	
	origin = Vec3{};
	horizontal = Vec3{viewportWidth, 0, 0};
	vertical = Vec3{0, viewportHeight, 0};
	lowerLeftCorner = origin - horizontal / 2 - vertical / 2 
		- Vec3{0, 0, focalLength};
}

auto Camera::ray(float u, float v) const -> Ray {
	return Ray{origin, lowerLeftCorner 
			+ u * horizontal 
			+ v * vertical 
			- origin};
}
