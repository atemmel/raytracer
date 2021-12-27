#include "ray.hpp"

Ray::Ray(Vec3 origin, Vec3 direction) : origin(origin), 
	direction(direction) {

}

auto Ray::at(float step) const -> Vec3 {
	return origin + (step * direction);
}
