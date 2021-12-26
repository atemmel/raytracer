#pragma once

#include "vec3.hpp"

#include <string_view>
#include <vector>

struct Image {
	static auto create(size_t w, size_t h) -> Image;

	auto writeToPpm(std::string_view path) const -> void;

	auto operator()(size_t x, size_t y) -> Vec3&;
	auto operator()(size_t x, size_t y) const -> const Vec3&;
	auto get(size_t x, size_t y) -> Vec3&;
	auto get(size_t x, size_t y) const -> const Vec3&;
	std::vector<Vec3> data;
	size_t width, height;
};
