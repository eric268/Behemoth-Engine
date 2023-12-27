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

			transformComp->dirty = true;
			const Math::Matrix4x4 rotationMatrix = Math::Matrix4x4::GetRotationMatrix(rotationComp->axis, rotationComp->speed);
			Math::Matrix4x4 newMatrix = rotationMatrix * transformComp->transformMatrix;

			for (int col = 0; col < 3; col++)
			{
				for (int row = 0; row < 3; row++)
				{
					transformComp->transformMatrix.data[col][row] = newMatrix.data[col][row];
				}
			}

			transformComp->forwardVector = GetForwardVector(transformComp->transformMatrix);
			transformComp->rightVector = GetRightVector(transformComp->transformMatrix);

			CameraComponent* cameraComponent = registry.GetComponent<CameraComponent>(entity);


			if (cameraComponent)
			{
				cameraComponent->isDirty = true;
			}

			MeshComponent* meshComp = registry.GetComponent<MeshComponent>(entity);
			if (meshComp)
			{
				for (int i = 0; i < meshComp->mesh.meshPrimitives.size(); i++)
				{
					for (int j = 0; j < 3; j++)
					{
						Math::Vector3::RotateVector(meshComp->mesh.meshPrimitives[i].normals[j], rotationMatrix);
					}
				}
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