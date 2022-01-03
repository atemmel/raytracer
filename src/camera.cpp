#include "camera.hpp"

#include "math.hpp"

Camera::Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 up, float verticalFov, float aspectRatio, float aperture, float focusDistance) {
	auto theta = degreesToRadians(verticalFov);
	auto h = std::tan(theta / 2.f);
	auto viewportHeight = 2.f * h;
	auto viewportWidth = aspectRatio * viewportHeight;

	// build coordinate frame
	w = (lookFrom - lookAt).unit();
	u = up.cross(w).unit();
	v = w.cross(u);
	
	origin = lookFrom;
	horizontal = focusDistance * viewportWidth* u;
	vertical = focusDistance * viewportHeight * v;
	lowerLeftCorner = origin - horizontal / 2 - vertical / 2 
		- focusDistance * w;
	radius = aperture / 2.f;
}

auto Camera::ray(float a, float b) const -> Ray {
	Vec3 vec{};
	randomUnitDisk(vec.x, vec.y);
	Vec3 randomWithinRadius = radius * vec;
	Vec3 offset = u * randomWithinRadius.x + v * randomWithinRadius.y;
	return Ray{
		origin + offset, 
		lowerLeftCorner 
		+ a * horizontal 
		+ b * vertical 
		- origin - offset};
}
