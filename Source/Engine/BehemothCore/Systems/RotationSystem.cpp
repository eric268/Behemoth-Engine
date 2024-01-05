#include "pch.h"
#include "RotationSystem.h"
#include "Components/Components.h"
#include "Misc/Log.h"
#include "ECS/Entity.h"

namespace Behemoth
{
	void RotationSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<RotationComponent, TransformComponent>();

		for (const auto& [entity, rotationComp, transformComp] : components)
		{
			if (rotationComp->axis == RotationComponent::None || rotationComp->speed == 0.0f)
			{
				continue;
			}

			const Math::Matrix4x4 rotationMatrix = Math::Matrix4x4::GetRotationMatrix(rotationComp->axis, rotationComp->speed);

			RotateTransformMatrix(transformComp, rotationMatrix);

			// If this entity has a camera component we need to update the view matrix as well after a rotation
			CameraComponent* cameraComponent = registry.GetComponent<CameraComponent>(entity);
			if (cameraComponent)
			{
				cameraComponent->isDirty = true;
			}

			// If this entity has a mesh component then we need to update the mesh normals after a rotation
			MeshComponent* meshComp = registry.GetComponent<MeshComponent>(entity);
			if (meshComp)
			{
				RotateMeshNormals(meshComp, rotationMatrix);
			}
		}
	}

	void RotationSystem::RotateTransformMatrix(TransformComponent* transformComponent, const Math::Matrix4x4& rotationMatrix)
	{
		Math::Matrix4x4 rotatedTransformMatrix = rotationMatrix * transformComponent->transformMatrix;
		for (int col = 0; col < 3; col++)
		{
			for (int row = 0; row < 3; row++)
			{
				transformComponent->transformMatrix.data[col][row] = rotatedTransformMatrix.data[col][row];
			}
		}
		transformComponent->forwardVector = GetForwardVector(transformComponent->transformMatrix);
		transformComponent->rightVector = GetRightVector(transformComponent->transformMatrix);
		transformComponent->upVector = Math::Vector3::Up();
		transformComponent->isDirty = true;
	}

	void RotationSystem::RotateMeshNormals(MeshComponent* meshComponent, const Math::Matrix4x4& rotationMatrix)
	{
		for (int i = 0; i < meshComponent->mesh.meshPrimitives.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Math::Vector3::RotateVector(meshComponent->mesh.meshPrimitives[i].normals[j], rotationMatrix);
			}
		}
	}

	Math::Vector3 RotationSystem::GetForwardVector(const Math::Matrix4x4& transformMatrix)
	{
		return  Math::Vector3(-transformMatrix._13, -transformMatrix._23, -transformMatrix._33).Normalize();
	}
	Math::Vector3 RotationSystem::GetRightVector(const Math::Matrix4x4& transformMatrix)
	{
		return Math::Vector3(transformMatrix._11, transformMatrix._21, transformMatrix._31).Normalize();
	}
}