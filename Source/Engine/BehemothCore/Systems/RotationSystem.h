#pragma once
#include "ECS/System.h"
#include "ECS/Entity.h"

namespace Behemoth
{
	struct TransformComponent;
	struct MeshComponent;

	// Debug
	class Primitives;

	class RotationSystem
	{
	public:
		RotationSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		BMath::Vector3 GetForwardVector(const BMath::Matrix4x4f& transformMatrix);
		BMath::Vector3 GetUpVector(const BMath::Matrix4x4f& transformMatrix);
		BMath::Vector3 GetRightVector(const BMath::Matrix4x4f& transformMatrix);

		void ApplyRotation(BMath::Matrix4x4f& transform, const BMath::Matrix4x4f& rotationMatrix);
		void UpdateWorldRotation(ECS::Registry& registry, const ECS::EntityHandle& handle, TransformComponent* transformComponent, const BMath::Matrix4x4f& rotationMatrix);
		void RotateMeshNormals(MeshComponent* meshComponent, const BMath::Matrix4x4f& rotationMatrix);
		void UpdateMeshNormalsFromParentRotation(TransformComponent* transformComp, MeshComponent* meshComponent);

		// DEBUG
		bool doOnce = true;
		int counter = 0;
		ECS::EntityHandle entityHandles[12];
		BMath::Vector3 GetPrimitivePosition(Primitives* primitive);
	};
}
