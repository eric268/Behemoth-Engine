#include "pch.h"
#include "Ray.h"

namespace Behemoth
{
	Ray::Ray() {}
	Ray::Ray(Math::Vector3 position, Math::Vector3 direction) : origin(position), direction(direction) {}

}