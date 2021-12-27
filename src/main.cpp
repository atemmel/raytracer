#include "camera.hpp"
#include "image.hpp"
#include "math.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "world.hpp"

#include <iostream>

auto rayColor(const Ray& ray, const World& world) -> Vec3 {
	RayHitData data{};
	if(ray.hit(world, 0.f, Infinity, data)) {
		return 0.5f * (data.normal + Vec3{1.f, 1.f, 1.f});
	}

	auto directionUnit = ray.direction.unit();
	auto step = 0.5f * directionUnit.y + 1.f;
	return (1.f - step) * Vec3{1.f, 1.f, 1.f}
		+ step * Vec3{0.5f, 0.7f, 1.f};
}

auto normalizeColor(Vec3 color, size_t samples) -> Vec3 {
	float scale = 1.f / samples;
	color = color * scale;
	color.r = clamp(color.r, 0.f, 0.999f);
	color.g = clamp(color.g, 0.f, 0.999f);
	color.b = clamp(color.b, 0.f, 0.999f);
	return color;
}

auto main() -> int {
	Camera camera;
	const size_t imageWidth = 400;
	const size_t imageHeight = 
		static_cast<int>(imageWidth / camera.aspectRatio);

	const size_t samplesPerPixel = 10;

	World world;
	world.add(Sphere{{0.f, 0.f, -1.f}, 0.5f});
	world.add(Sphere{{0.f, -100.5f, -1.f}, 100.f});

	auto image = Image::create(imageWidth, imageHeight);

	for(size_t x = 0; x < image.width; x++) {
		for(size_t y = 0; y < image.height; y++) {
			size_t yn = image.height - 1 - y;
			Vec3 color{};
			for(size_t sample = 0; sample < samplesPerPixel; sample++) {
				auto u = float(x + randomFloat()) / (image.width - 1);
				auto v = float(yn + randomFloat()) / (image.height - 1);
				auto ray = camera.ray(u, v);
				color = color + rayColor(ray, world);

			}
			image(x, y) = normalizeColor(color, samplesPerPixel);
		}
	}

	image.writeToPpm("image.ppm");
}
