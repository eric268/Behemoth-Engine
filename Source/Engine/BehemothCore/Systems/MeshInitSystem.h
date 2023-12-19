#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class Mesh;

	class MeshInitSystem
	{
	public:
		MeshInitSystem() = default;

		void Run(ECS::Registry& registry);

	private:
		void InitMesh(Mesh& mesh);
	};

}