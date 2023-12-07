#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class RotationSystem : public ECS::System
	{
	public:
		RotationSystem() = default;

		virtual void Run(ECS::Registry& registry) override;
	};
}
