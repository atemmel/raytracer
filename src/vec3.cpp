#include "vec3.hpp"

#include <cmath>

Vec3::Vec3() : Vec3(0.f, 0.f, 0.f) {

}

Vec3::Vec3(float f0, float f1, float f2) : x(f0), y(f1), z(f2) {
}

auto Vec3::operator-() const -> Vec3 {
	return Vec3(-x, -y, -z);
}

auto Vec3::operator[](int index) const -> float {
	return (&x)[index];
}

auto Vec3::operator[](int index) -> float& {
	return (&x)[index];
}

auto Vec3::operator+(const Vec3 rhs) const -> Vec3 {
	return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

auto Vec3::operator-(const Vec3 rhs) const -> Vec3 {
	return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
}

auto Vec3::operator*(float e) const -> Vec3 {
	return Vec3(x * e, y * e, z * e);
}

auto Vec3::operator/(float e) const -> Vec3 {
	return Vec3(x / e, y / e, z / e);
}

auto Vec3::dot(const Vec3 rhs) const -> float {
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

auto Vec3::cross(const Vec3 rhs) const -> Vec3 {
	return Vec3(
		y * rhs.z - z * rhs.y,
		z * rhs.x - x * rhs.z,
		x * rhs.y - y * rhs.x);
}

auto Vec3::norm() const -> float {
	return std::sqrt(x*x + y*y + z*z);
}

auto Vec3::unit() const -> Vec3 {
	return *this / norm();
}
