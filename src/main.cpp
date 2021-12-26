#include "image.hpp"

#include <iostream>

auto main() -> int {
	auto image = Image::create(400, 300);

	for(size_t x = 0; x < image.width; x++) {
		for(size_t y = 0; y < image.height; y++) {
			size_t yn = image.height - 1 - y;
			const float r = float(x) / (image.width - 1);
			const float g = float(yn) / (image.height - 1);
			const float b = 0.25f;

			image(x, y) = {r, g, b};
		}
	}

	image.writeToPpm("image.ppm");
}
