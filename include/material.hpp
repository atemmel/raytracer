#pragma once
#include "ray.hpp"
#include "vec3.hpp"

#include <vector>

struct Materials {
	struct Material {
		enum Model {
			Lambertian,
		};

		Model model;

		struct LambertianMaterial {
			Vec3 albedo;
		};

		union {
			LambertianMaterial lambertian;
		};
	};
	
	static auto create(const Material& material) -> size_t;

	static auto apply(size_t index, RayHitData& data, Vec3& result, Ray& scattered) -> bool;

private:
	static auto apply(const Material::LambertianMaterial& material, RayHitData& data, Vec3& result, Ray& scattered) -> bool;

	static std::vector<Material> materials;
};
