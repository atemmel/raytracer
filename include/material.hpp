#pragma once
#include "ray.hpp"
#include "vec3.hpp"

#include <vector>

struct Materials {
	struct Material {
		enum Model {
			Lambertian,
			Metallic,
		};

		Model model;

		struct LambertianMaterial {
			Vec3 albedo;
		};

		struct MetallicMaterial {
			Vec3 albedo;
			float fuzziness;
		};

		union {
			LambertianMaterial lambertian;
			MetallicMaterial metallic;
		};
	};
	
	static auto create(const Material& material) -> size_t;

	static auto apply(size_t index, const Ray& in, RayHitData& data, Vec3& result, Ray& scattered) -> bool;

private:
	static auto apply(const Material::LambertianMaterial& material, const Ray& in, RayHitData& data, Vec3& result, Ray& scattered) -> bool;
	static auto apply(const Material::MetallicMaterial& material, const Ray& in, RayHitData& data, Vec3& result, Ray& scattered) -> bool;

	static std::vector<Material> materials;
};
