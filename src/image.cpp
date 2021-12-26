#include "image.hpp"

auto Image::create(size_t w, size_t h) -> Image {
	Image image;
	image.width = w;
	image.height = h;
	image.data.resize(w * h, {0.f, 0.f, 0.f});
	return image;
}

auto Image::operator()(size_t x, size_t y) -> Color& {
	return data[y * width + x];
}

auto Image::operator()(size_t x, size_t y) const -> const Color& {
	return data[y * width + x];
}
