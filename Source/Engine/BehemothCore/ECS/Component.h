#pragma once

#include "Generator.h"
#include "Math/MathCore.h"

namespace ECS
{
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() {}
	}; 
}