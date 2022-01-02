#include "material.hpp"

std::vector<Materials::Material> Materials::materials;

auto Materials::create(const Material& material) -> size_t {
	materials.push_back(material);
	return materials.size() - 1;
}

auto Materials::apply(size_t index, RayHitData& data, Vec3& result, Ray& scattered) -> bool {
	const Material& material = materials[index];
	switch(material.model) {
		case Material::Lambertian:
			return applyLambertian(material.lambertian, data, result, scattered);
	}

	return false;
}

auto Materials::applyLambertian(const Material::LambertianMaterial& material, RayHitData& data, Vec3& result, Ray& scattered) -> bool {
	Vec3 direction = data.normal + Vec3::randomUnit();

	if(direction.nearZero()) {
		direction = data.normal;
	}

	scattered.origin = data.point;
	scattered.direction = direction;
	result = material.albedo;
	return true;
}
