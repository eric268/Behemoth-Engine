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