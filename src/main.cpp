#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <SDL2/SDL.h>

#include "box.hpp"
#include "camera.hpp"
#include "image.hpp"
#include "material.hpp"
#include "math.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "world.hpp"

#include <iostream>
#include <random>
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

auto createShapes(size_t count, World& world, const Box& floor) {
	const float metalMaterialThreshold = 0.80f;
	const float boxThreshold = 0.7f;
	for(size_t i = 0; i < count; i++) {
		auto randomMaterial = randomFloat() < metalMaterialThreshold
			? Materials::Material{
				.model = Materials::Material::Lambertian,
				.lambertian = {
					.albedo = {
						randomFloat(),
						randomFloat(),
						randomFloat(),
					},
				},
			}
			: Materials::Material{
				.model = Materials::Material::Metallic,
				.metallic = {
					.albedo = {
						randomFloat(0.5f, 1.0f),
						randomFloat(0.5f, 1.0f),
						randomFloat(0.5f, 1.0f),
					},
					.fuzziness = randomFloat(0.f, 0.5f),
				},
			};

		auto material = Materials::create(randomMaterial);

		float dimension = randomFloat(0.01f, 0.5f);
		if(randomFloat() < boxThreshold) {
			auto dimensions = Vec3{dimension, dimension, dimension};

			auto min = Vec3{
				randomFloat(-2.f, 2.f),
				0.f,
				randomFloat(-5.f, 5.f),
			};

			min.y = floor.max.y;

			auto box = Box{
				.min = min,
				.max = min + dimensions,
				.material = material,
			};

			world.add(box);
		} else {
			auto origin = Vec3{
				randomFloat(-2.f, 2.f),
				0.f,
				randomFloat(-5.f, 5.f),
			};

			origin.y = floor.max.y + dimension;

			auto sphere = Sphere{
				.origin = origin,
				.radius = dimension,
				.material = material,
			};
			world.add(sphere);
		}
	}
}

Image image;
SDL_Surface* surface;
SDL_Window* window;

void everyTick() {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					SDL_DestroyWindow(window);
					SDL_Quit();
					std::exit(0);
					break;
			}
		}

		if(SDL_MUSTLOCK(surface)) {
			SDL_LockSurface(surface);
		}
		uint8_t* pixel = reinterpret_cast<uint8_t*>(surface->pixels);
		size_t j = 0;
		for(size_t i = 0; i < image.data.size(); i++) {
			pixel[j + 0] = static_cast<uint8_t>(image.data[i].b * 255.999);
			pixel[j + 1] = static_cast<uint8_t>(image.data[i].g * 255.999);
			pixel[j + 2] = static_cast<uint8_t>(image.data[i].r * 255.999);
			j += 4;
		}
		SDL_UpdateWindowSurface(window);
		SDL_UnlockSurface(surface);
}

auto main() -> int {

	const Vec3 lookFrom = {
		0.f, 1.f, 11.f,
	};

	const Vec3 lookAt = {
		0.f, 0.f, -0.f, 
	};

	const Vec3 up = {
		0.f, 1.f, 0.f,
	};

	const float fov = 20.f;
	const float aspectRatio = 16.f / 9.f;
	auto aperture = 0.1f;
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
	const size_t imageWidth = 1200;
	const size_t imageHeight = 
		static_cast<int>(imageWidth / aspectRatio);

	const size_t samplesPerPixel = 200;

	const size_t maxDepth = 20;

	auto floorMaterial= Materials::create({
		.model = Materials::Material::Lambertian,
		.lambertian = {
			.albedo = {0.3f, 0.4f, 0.5f},
		},
	});

	World world;
	auto floor = Box{
		.min = {-100.f, -10.f, -100.f},
		.max = { 100.f, -0.5f,  100.f},
		.material = floorMaterial,
	};
	world.add(floor);
	createShapes(20, world, floor);

	image = Image::create(imageWidth, imageHeight);
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

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Result",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			image.width, image.height, SDL_WINDOW_SHOWN);

	surface = SDL_GetWindowSurface(window);

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(everyTick, 30, 1);
#else
	while(true) {
		everyTick();
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(30ms);
	}
#endif
}
