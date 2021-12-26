#pragma once

#include "color.hpp"

#include <string_view>
#include <vector>

struct Image {
	static auto create(size_t w, size_t h) -> Image;

	auto writeToPpm(std::string_view path) const -> void;

	auto operator()(size_t x, size_t y) -> Color&;
	auto operator()(size_t x, size_t y) const -> const Color&;
	auto get(size_t x, size_t y) -> Color&;
	auto get(size_t x, size_t y) const -> const Color&;
	std::vector<Color> data;
	size_t width, height;
};
