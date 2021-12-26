#pragma once

#include "color.hpp"

#include <vector>

struct Image {
	static auto create(size_t w, size_t h) -> Image;

	auto operator()(size_t x, size_t y) -> Color&;
	auto operator()(size_t x, size_t y) const -> const Color&;
	std::vector<Color> data;
	size_t width, height;
};
