#include "pch.h"
#include "Ray.h"

namespace Behemoth
{
	Ray::Ray() {}
	Ray::Ray(BMath::Vector3 position, BMath::Vector3 direction) : origin(position), direction(direction) {}

}