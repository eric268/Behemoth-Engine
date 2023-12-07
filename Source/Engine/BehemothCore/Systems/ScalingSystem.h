#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class ScalingSystem : public ECS::System
	{
	public:
		ScalingSystem() = default;

		virtual void Run(ECS::Registry& registry) override;
	};
}

