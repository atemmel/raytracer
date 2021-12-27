#include "image.hpp"
#include "ray.hpp"

#include <iostream>

auto rayColor(const Ray& ray) -> Vec3 {
	auto unit = ray.direction.unit();
	auto t = 0.5f * (unit.y + 1.f);
	return (1.f - t) * Vec3(1.f, 1.f, 1.f) 
		+ t * Vec3(0.5f, 0.7f, 1.f);
}

auto main() -> int {
	const auto aspectRatio = 16.f / 9.f;
	const size_t imageWidth = 400;
	const size_t imageHeight = 
		static_cast<int>(imageWidth / aspectRatio);

	auto viewportHeight = 2.f;
	auto viewportWidth = aspectRatio * viewportHeight;
	auto focalLength = 1.f;

	auto origin = Vec3();
	auto horizontal = Vec3(viewportWidth, 0, 0);
	auto vertical = Vec3(0, viewportHeight, 0);
	auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2 
		- Vec3(0, 0, focalLength);

	auto image = Image::create(imageWidth, imageHeight);

	for(size_t x = 0; x < image.width; x++) {
		for(size_t y = 0; y < image.height; y++) {
			size_t yn = image.height - 1 - y;
			auto u = float(x) / (image.width - 1);
			auto v = float(y) / (image.height - 1);
			auto ray = Ray(origin, lowerLeftCorner
					+ u * horizontal
					+ v * vertical);

			auto color = rayColor(ray);
			image(x, y) = color;
		}
	}

	/*
	for(size_t x = 0; x < image.width; x++) {
		for(size_t y = 0; y < image.height; y++) {
			size_t yn = image.height - 1 - y;
			const float r = float(x) / (image.width - 1);
			const float g = float(yn) / (image.height - 1);
			const float b = 0.25f;

			image(x, y) = {r, g, b};
		}
	}
	*/

	image.writeToPpm("image.ppm");
}
