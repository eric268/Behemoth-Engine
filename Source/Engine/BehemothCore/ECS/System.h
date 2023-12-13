#pragma once
#include "Registry.h"

namespace ECS
{
	class System
	{
	public:
		virtual ~System() = default;
		virtual void Run(Registry& registry) = 0;
	};
}
