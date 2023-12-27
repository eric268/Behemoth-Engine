#include "pch.h"
#include "RotationSystem.h"
#include "Components/Components.h"
#include "Misc/Log.h"
#include "ECS/Entity.h"

namespace Behemoth
{
	void RotationSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<RotationComponent, TransformComponent, MeshComponent>();

		for (const auto& [entity, rotationComp, transformComp, meshComp] : components)
		{
			if (!rotationComp || !transformComp || !meshComp)
			{
				LOG_ERROR("Null component found");
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

			transformComp->forwardVector =	GetForwardVector(transformComp->transformMatrix);
			transformComp->rightVector =	GetRightVector(transformComp->transformMatrix);

			for (int i = 0; i < meshComp->mesh.meshPrimitives.size(); i++)
			{
				for (int j = 0; j < 3; j++)
				{
					Math::Vector3::RotateVector(meshComp->mesh.meshPrimitives[i].normals[j], rotationMatrix);
				}
			}
		}
	}

	Math::Vector3 RotationSystem::GetForwardVector(const Math::Matrix4x4& transformMatrix)
	{
		return Math::Vector3(transformMatrix.data[0][2], transformMatrix.data[1][2], transformMatrix.data[2][2]);
	}
	Math::Vector3 RotationSystem::GetRightVector(const Math::Matrix4x4& transformMatrix)
	{
		return Math::Vector3(transformMatrix.data[0][0], transformMatrix.data[1][0], transformMatrix.data[2][0]);
	}
}