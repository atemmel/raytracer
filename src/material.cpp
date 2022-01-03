#include "material.hpp"

std::vector<Materials::Material> Materials::materials;

auto Materials::create(const Material& material) -> size_t {
	materials.push_back(material);
	return materials.size() - 1;
}

auto Materials::apply(size_t index, const Ray& ray, RayHitData& data, Vec3& result, Ray& scattered) -> bool {
	const Material& material = materials[index];
	switch(material.model) {
		case Material::Lambertian:
			return apply(material.lambertian, ray, data, result, scattered);
		case Material::Metallic:
			return apply(material.metallic, ray, data, result, scattered);
	}

	return false;
}

auto Materials::apply(const Material::LambertianMaterial& material, const Ray& ray, RayHitData& data, Vec3& result, Ray& scattered) -> bool {
	Vec3 direction = data.normal + Vec3::randomUnit();

	if(direction.nearZero()) {
		direction = data.normal;
	}

	scattered.origin = data.point;
	scattered.direction = direction;
	result = material.albedo;
	return true;
}

auto Materials::apply(const Material::MetallicMaterial& material, const Ray& ray, RayHitData& data, Vec3& result, Ray& scattered) -> bool {
	Vec3 reflected = ray.direction.unit().reflect(data.normal);
	scattered = Ray{
		.origin = data.point,
		.direction = reflected + material.fuzziness * Vec3::randomUnit(),
	};

	result = material.albedo;
	return scattered.direction.dot(data.normal) > 0.f;
}
