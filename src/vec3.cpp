#include "vec3.hpp"

#include <cmath>

auto Vec3::operator-() const -> Vec3 {
	return {-x, -y, -z};
}

auto Vec3::operator[](int index) const -> float {
	return (&x)[index];
}

auto Vec3::operator[](int index) -> float& {
	return (&x)[index];
}

auto Vec3::operator+(const Vec3 rhs) const -> Vec3 {
	return {x + rhs.x, y + rhs.y, z + rhs.z};
}

auto Vec3::operator-(const Vec3 rhs) const -> Vec3 {
	return {x - rhs.x, y - rhs.y, z - rhs.z};
}

auto Vec3::operator*(float e) const -> Vec3 {
	return {x * e, y * e, z * e};
}

auto Vec3::operator/(float e) const -> Vec3 {
	return {x / e, y / e, z / e};
}

auto Vec3::dot(const Vec3 rhs) const -> float {
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

auto Vec3::cross(const Vec3 rhs) const -> Vec3 {
	return {
		y * rhs.z - z * rhs.y,
		z * rhs.x - x * rhs.z,
		x * rhs.y - y * rhs.x};
}

auto Vec3::squaredNorm() const -> float {
	return x*x + y*y + z*z;
}

auto Vec3::norm() const -> float {
	return std::sqrt(squaredNorm());
}

auto Vec3::unit() const -> Vec3 {
	return *this / norm();
}

auto operator*(float lhs, Vec3 rhs) -> Vec3 {
	return rhs * lhs;
}

auto operator/(float lhs, Vec3 rhs) -> Vec3 {
	return rhs * lhs;
}
