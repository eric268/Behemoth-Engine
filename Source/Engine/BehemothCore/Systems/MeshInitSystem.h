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
	struct MeshInitalizeComponent;
	struct AABBCollider;

	class MeshInitSystem
	{
	public:
		MeshInitSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void InitMesh(Mesh& mesh);

/*		void GenerateAABBBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle);*/
		void InitSphereBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle);

// 		void InitAABBBoundingVolume(ECS::Registry&, MeshComponent* meshComp, const ECS::EntityHandle& hanlde);
// 
// 		void GetRotatedAABB(const AABBCollider& a, const BMath::Matrix3x3& rotation, AABBCollider& result);
	};

}