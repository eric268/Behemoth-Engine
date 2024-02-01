#pragma once

#include "Generator.h"

namespace ECS
{
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() {}
	}; 
}