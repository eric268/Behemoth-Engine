#pragma once
#include "ECS/System.h"
#include "ECS/Entity.h"

namespace Behemoth
{
	class TransformComponent;
	class MeshComponent;

	// Debug
	class Primitives;

	class RotationSystem
	{
	public:
		RotationSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		BMath::Vector3 GetForwardVector(const BMath::Matrix4x4& transformMatrix);
		BMath::Vector3 GetRightVector(const BMath::Matrix4x4& transformMatrix);

		void RotateTransformMatrix(TransformComponent* transformComponent, const BMath::Matrix4x4& rotationMatrix);
		void RotateMeshNormals(MeshComponent* meshComponent, const BMath::Matrix4x4& rotationMatrix);
	};
}
