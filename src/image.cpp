#include "image.hpp"

#include <fstream>

auto Image::create(size_t w, size_t h) -> Image {
	Image image;
	image.width = w;
	image.height = h;
	image.data.resize(w * h, Vec3{0.f, 0.f, 0.f});
	return image;
}

auto Image::writeToPpm(std::string_view path) const -> void {
	std::ofstream file(path.data());
	file << "P3\n" << width << ' ' << height
		<< "\n255\n";

	for(size_t y = 0; y < height; y++) {
		for(size_t x = 0; x < width; x++) {
			auto clr = get(x, y);
			int r = static_cast<int>(clr.r * 255.999);
			int g = static_cast<int>(clr.g * 255.999);
			int b = static_cast<int>(clr.b * 255.999);
			file << r << ' ' << g << ' ' << b << '\n';
		}
	}
}

auto Image::operator()(size_t x, size_t y) -> Vec3& {
	return get(x, y);
}

auto Image::operator()(size_t x, size_t y) const -> const Vec3& {
	return get(x, y);
}

auto Image::get(size_t x, size_t y) -> Vec3& {
	return data[y * width + x];
}

auto Image::get(size_t x, size_t y) const -> const Vec3& {
	return data[y * width + x];
}
