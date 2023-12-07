#pragma once

#include "Generator.h"
#include "Render/Mesh.h"
#include "Math/MathCore.h"

// stl
#include <iostream>

namespace ECS
{
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() {}
	}; 
}