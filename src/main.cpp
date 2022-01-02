#include "box.hpp"
#include "camera.hpp"
#include "image.hpp"
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
		Vec3 target = data.point + data.normal + Vec3::randomUnit();
		return 0.5f * rayColor(Ray{data.point, target - data.point}, world, depth - 1);
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

	Camera camera;
	constexpr size_t imageWidth = 800;
	const size_t imageHeight = 
		static_cast<int>(imageWidth / camera.aspectRatio);

	constexpr size_t samplesPerPixel = 100;

	constexpr size_t maxDepth = 20;

	World world;
	world.add(Sphere{{0.f, 0.f, -1.f}, 0.5f});
	world.add(Sphere{{2.f, 0.f, -2.f}, 0.2f});
	world.add(Sphere{{0.2f, 0.f, -0.6f}, 0.2f});
	world.add(Sphere{{0.f, -100.5f, -1.f}, 100.f});

	world.add(Box{
		.min = {-0.5f, 0.0f, -1.0f}, 
		.max = {-0.0f, 0.5f, -0.5f}
	});

	auto image = Image::create(imageWidth, imageHeight);
	image.writeToPpm("image.ppm");
	if(!singleThread) {
		std::mutex imageMutex;
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
			imageMutex.lock();
			image.writeToPpm("image.ppm");
			imageMutex.unlock();
			completedThreads += 1;
		};

		size_t nThreads = 8;

		std::vector<std::thread> threads;
		threads.reserve(nThreads);
		size_t colsPerThread = image.width / nThreads;
		for(size_t x = 0; x < image.width; x += colsPerThread) {
			//threads[x] = std::thread(doColumns, x, x + colsPerThread);
			threads.emplace_back(doColumns, x, x + colsPerThread);
			threads.back().detach();
		}

		while(completedThreads < nThreads) {
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(30ms);
		}
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
