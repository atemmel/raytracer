#include "box.hpp"
#include "camera.hpp"
#include "image.hpp"
#include "material.hpp"
#include "math.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "world.hpp"

#include <iostream>
#include <thread>

auto rayColor(const Ray& ray, const World& world, size_t depth) -> Vec3 {
	if(depth == 0) {
		return {};
	}

	RayHitData data{};
	if(ray.hit(world, 0.0001f, Infinity, data)) {
		Ray scattered{};
		Vec3 color{};
		if(Materials::apply(data.material, ray, data, color, scattered)) {
			return color * rayColor(scattered, world, depth - 1);
		}
		return {};
	}

	auto directionUnit = ray.direction.unit();
	auto step = 0.5f * directionUnit.y + 1.f;
	return (1.f - step) * Vec3{1.f, 1.f, 1.f}
		+ step * Vec3{0.5f, 0.7f, 1.f};
}

auto normalizeColor(Vec3 color, size_t samples) -> Vec3 {
	float scale = 1.f / samples;
	color = color * scale;
	color.r = std::sqrt(color.r);
	color.g = std::sqrt(color.g);
	color.b = std::sqrt(color.b);

	color.r = clamp(color.r, 0.f, 0.999f);
	color.g = clamp(color.g, 0.f, 0.999f);
	color.b = clamp(color.b, 0.f, 0.999f);
	return color;
}

auto main(int argc, char** argv) -> int {
	bool singleThread = false;
	if(argc > 1) {
		if(argv[1] == std::string_view("--no-thread")) {
			singleThread = true;
		}
	}

	constexpr Vec3 lookFrom = {
		//13.f, 2.f, 3.f,
		0.f, 2.f, 13.f,
	};

	constexpr Vec3 lookAt = {
		0.f, 0.f, -0.f, 
	};

	constexpr Vec3 up = {
		0.f, 1.f, 0.f,
	};

	constexpr float fov = 20.f;
	constexpr float aspectRatio = 16.f / 9.f;
	auto aperture = 0.1f;
	//auto focusDistance = (lookFrom - lookAt).norm();
	auto focusDistance = 10.f;

	Camera camera(
		lookFrom,
		lookAt,
		up,
		fov,
		aspectRatio,
		aperture,
		focusDistance
		
	);
	constexpr size_t imageWidth = 1920;
	const size_t imageHeight = 
		static_cast<int>(imageWidth / aspectRatio);

	constexpr size_t samplesPerPixel = 500;

	constexpr size_t maxDepth = 20;

	auto blueMaterial = Materials::create({
		.model = Materials::Material::Lambertian,
		.lambertian = {
			.albedo = {0.4f, 0.4f, 0.6f},
		},
	});

	auto redMaterial = Materials::create({
		.model = Materials::Material::Lambertian,
		.lambertian = {
			.albedo = {1.0f, 0.0f, 0.0f},
		},
	});

	auto greenMaterial = Materials::create({
		.model = Materials::Material::Lambertian,
		.lambertian = {
			.albedo = {0.2f, 0.8f, 0.1f},
		},
	});

	auto metalMaterial = Materials::create({
		.model = Materials::Material::Metallic,
		.metallic = {
			.albedo = {0.8f, 0.8f, 0.8f},
			.fuzziness = 0.f,
		},
	});

	auto fuzzyMetalMaterial = Materials::create({
		.model = Materials::Material::Metallic,
		.metallic = {
			.albedo = {0.2f, 0.2f, 0.2f},
			.fuzziness = 0.5f,
		},
	});

	World world;
	world.add(Sphere{{0.f, 0.f,  2.f}, 0.5f, metalMaterial});
	world.add(Sphere{{2.f, 0.f, -1.f}, 0.2f, redMaterial});
	world.add(Sphere{{1.f, -0.3f, -1.5f}, 0.2f, fuzzyMetalMaterial});
	world.add(Sphere{{-0.7f, 0.5f, -1.1f}, 0.2f, blueMaterial});
	world.add(Sphere{{0.f, -100.5f, -1.f}, 100.f, greenMaterial});

	world.add(Box{
		.min = {-1.5f, -0.5f, -1.5f}, 
		.max = {-1.0f,  0.0f, -1.0f},
		.material = blueMaterial,
	});

	auto image = Image::create(imageWidth, imageHeight);
	if(!singleThread) {
		std::atomic_int completedThreads = 0;

		auto doColumns = [&](size_t xmin, size_t xmax) {
			for(size_t x = xmin; x < xmax; x++) {
				for(size_t y = 0; y < image.height; y++) {
					size_t yn = image.height - 1 - y;
					Vec3 color{};
					for(size_t sample = 0; sample < samplesPerPixel; sample++) {
						auto u = float(x + randomFloat()) / (image.width - 1);
						auto v = float(yn + randomFloat()) / (image.height - 1);
						auto ray = camera.ray(u, v);
						color = color + rayColor(ray, world, maxDepth);

					}
					image(x, y) = normalizeColor(color, samplesPerPixel);
				}
			}
			completedThreads += 1;
		};

		size_t nThreads = std::thread::hardware_concurrency();

		std::vector<std::thread> threads;
		threads.reserve(nThreads);
		size_t colsPerThread = image.width / nThreads;
		for(size_t x = 0; x < image.width; x += colsPerThread) {
			threads.emplace_back(doColumns, x, x + colsPerThread);
			threads.back().detach();
		}

		while(completedThreads < nThreads) {
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(30ms);
		}

		image.writeToPpm("image.ppm");
	} else {
		for(size_t x = 0; x < image.width; x++) {
			for(size_t y = 0; y < image.height; y++) {
				size_t yn = image.height - 1 - y;
				Vec3 color{};
				for(size_t sample = 0; sample < samplesPerPixel; sample++) {
					auto u = float(x + randomFloat()) / (image.width - 1);
					auto v = float(yn + randomFloat()) / (image.height - 1);
					auto ray = camera.ray(u, v);
					color = color + rayColor(ray, world, maxDepth);

				}
				image(x, y) = normalizeColor(color, samplesPerPixel);
			}
		}
		image.writeToPpm("image.ppm");
	}
}
