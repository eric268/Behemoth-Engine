#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class MeshInitSystem : public ECS::System
	{
	public:
		MeshInitSystem() = default;

		virtual void Run(ECS::Registry& registry) override;
	};

}