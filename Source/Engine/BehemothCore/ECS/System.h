#pragma once
#include "Registry.h"
#include "Render/Primitives.h"

namespace ECS
{
	class System
	{
	public:
		virtual ~System() = default;

		virtual void Run(Registry& registry) = 0;
	};
}
