#pragma once

#include <cmath>
#include <limits>

constexpr float Infinity = std::numeric_limits<float>::infinity();
constexpr float Pi = 3.1415927f;

auto degreesToRadians(float theta) -> float;
auto radiansToDegrees(float theta) -> float;

auto randomFloat(float min = 0.f, float max = 1.f) -> float;
auto randomUnitDisk(float &x, float &y) -> void;
auto clamp(float a, float min, float max) -> float;
