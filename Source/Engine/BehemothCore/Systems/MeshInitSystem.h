#pragma once
#include "Math/Matrix.h"

namespace ECS
{
	class Registry;
	class EntityHandle;
}

namespace Behemoth
{
	class Mesh;
	struct MeshComponent;
	struct MeshInitializeComponent;
	struct AABBCollider;

	class MeshInitSystem
	{
	public:
		MeshInitSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void InitMesh(Mesh& mesh);
		void InitSphereBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle);
	};

}