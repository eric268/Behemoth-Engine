#pragma once
#include "ECS/System.h"
#include "ECS/Entity.h"

namespace Behemoth
{
	struct TransformComponent;
	struct MeshComponent;
	class Primitive;

	class RotationSystem
	{
	public:
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		BMath::Vector3 GetForwardVector(const BMath::Matrix4x4& transformMatrix);
		BMath::Vector3 GetUpVector(const BMath::Matrix4x4& transformMatrix);
		BMath::Vector3 GetRightVector(const BMath::Matrix4x4& transformMatrix);

		void ApplyRotation(TransformComponent* transformComp, const BMath::Matrix4x4& rotationMatrix, bool isAdditive);
		void RotateMeshNormals(MeshComponent* meshComponent, const BMath::Matrix4x4& rotationMatrix);
		void UpdateMeshNormalsFromParentRotation(TransformComponent* transformComp, MeshComponent* meshComponent);
	};
}
