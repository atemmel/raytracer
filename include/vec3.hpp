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

	Vec3();
	Vec3(float f0, float f1, float f2);

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
	auto unit() const -> Vec3;
};
