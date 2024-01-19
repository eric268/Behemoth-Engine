#pragma once
#include "ECS/System.h"
#include "ECS/Entity.h"

namespace Behemoth
{
	struct TransformComponent;
	struct MeshComponent;

	// Debug
	class Primitive;

	class RotationSystem
	{
	public:
		RotationSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		BMath::Vector3 GetForwardVector(const BMath::BMatrix4x4& transformMatrix);
		BMath::Vector3 GetUpVector(const BMath::BMatrix4x4& transformMatrix);
		BMath::Vector3 GetRightVector(const BMath::BMatrix4x4& transformMatrix);

		void ApplyRotation(BMath::BMatrix4x4& transform, const BMath::BMatrix4x4& rotationMatrix);
		void UpdateWorldRotation(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComponent, const BMath::BMatrix4x4& rotationMatrix);
		void RotateMeshNormals(MeshComponent* meshComponent, const BMath::BMatrix4x4& rotationMatrix);
		void UpdateMeshNormalsFromParentRotation(TransformComponent* transformComp, MeshComponent* meshComponent);
	};
}
