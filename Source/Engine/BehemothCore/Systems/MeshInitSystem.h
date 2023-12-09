#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class Mesh;

	class MeshInitSystem : public ECS::System
	{
	public:
		MeshInitSystem() = default;

		virtual void Run(ECS::Registry& registry) override;

		void InitMesh(Mesh& mesh);
	};

}