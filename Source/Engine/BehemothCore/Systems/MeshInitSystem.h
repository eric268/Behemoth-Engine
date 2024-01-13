#pragma once
#include "ECS/Entity.h"
#include "Math/Matrix.h"

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	class Mesh;
	class MeshComponent;
	class MeshInitComponent;
	class AABBCollider;

	class MeshInitSystem
	{
	public:
		MeshInitSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void InitMesh(Mesh& mesh);

		void GenerateAABBBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle);
		void GenerateSphereBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle);

		void CalculateRotatedAABB(ECS::Registry&, MeshComponent* meshComp, const ECS::EntityHandle& hanlde);

		void GetRotatedAABB(const AABBCollider& a, const BMath::Matrix3x3& rotation, AABBCollider& result);
	};

}