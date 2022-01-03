#pragma once

struct Vec3 {
	union {
		struct {
			float r = 0, g = 0, b = 0;
		};
		struct {
			float x, y, z;
		};
	};

	static auto random(float min = 0.f, float max = 1.f) -> Vec3;
	static auto randomUnit() -> Vec3;

	auto operator-() const -> Vec3;
	auto operator[](int index) const -> float;
	auto operator[](int index) -> float&;
	auto operator+(const Vec3 rhs) const -> Vec3;
	auto operator-(const Vec3 rhs) const -> Vec3;
	auto operator*(float e) const -> Vec3;
	auto operator/(float e) const -> Vec3;

	auto dot(const Vec3 rhs) const -> float;
	auto cross(const Vec3 rhs) const -> Vec3;

	auto operator*(Vec3 rhs) const -> Vec3;

	auto norm() const -> float;
	auto squaredNorm() const -> float;
	auto unit() const -> Vec3;
	auto reflect(const Vec3 u) const -> Vec3;

	auto nearZero() const -> bool;
};

auto operator*(float lhs, Vec3 rhs) -> Vec3;
auto operator/(float lhs, Vec3 rhs) -> Vec3;
