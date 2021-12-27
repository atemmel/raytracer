#pragma once

struct Vec3 {
	union {
		struct {
			float r, g, b;
		};
		struct {
			float x, y, z;
		};
	};

	auto operator-() const -> Vec3;
	auto operator[](int index) const -> float;
	auto operator[](int index) -> float&;
	auto operator+(const Vec3 rhs) const -> Vec3;
	auto operator-(const Vec3 rhs) const -> Vec3;
	auto operator*(float e) const -> Vec3;
	auto operator/(float e) const -> Vec3;

	auto dot(const Vec3 rhs) const -> float;
	auto cross(const Vec3 rhs) const -> Vec3;

	auto norm() const -> float;
	auto squaredNorm() const -> float;
	auto unit() const -> Vec3;
};

auto operator*(float lhs, Vec3 rhs) -> Vec3;
auto operator/(float lhs, Vec3 rhs) -> Vec3;
