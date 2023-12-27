#pragma once
#include "ECS/System.h"

namespace Behemoth
{
	class TransformComponent;
	class MeshComponent;

	class RotationSystem
	{
	public:
		RotationSystem() = default;

		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		Math::Vector3 GetForwardVector(const Math::Matrix4x4& transformMatrix);
		Math::Vector3 GetRightVector(const Math::Matrix4x4& transformMatrix);

		void RotateTransformMatrix(TransformComponent* transformComponent, const Math::Matrix4x4& rotationMatrix);
		void RotateMeshNormals(MeshComponent* meshComponent, const Math::Matrix4x4& rotationMatrix);
	};
}
