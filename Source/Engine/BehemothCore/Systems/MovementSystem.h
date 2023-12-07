#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class MovementSystem : public ECS::System
	{
	public:
		MovementSystem() = default;

		virtual void Run(ECS::Registry& registry) override;
	};
}

