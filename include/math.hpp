#pragma once

#include <cmath>
#include <limits>

constexpr float Infinity = std::numeric_limits<float>::infinity();
constexpr float Pi = 3.1415927f;

constexpr auto degreesToRadians(float theta) -> float;
constexpr auto radiansToDegrees(float theta) -> float;

auto randomFloat(float min = 0.f, float max = 1.f) -> float;
auto clamp(float a, float min, float max) -> float;
