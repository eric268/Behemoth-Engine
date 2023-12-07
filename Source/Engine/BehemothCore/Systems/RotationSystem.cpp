#include "RotationSystem.h"
#include "Components/Components.h"
#include "Misc/Log.h"

namespace Behemoth
{
	void RotationSystem::Run(ECS::Registry& registry)
	{
		auto components = registry.Get<RotationComponent, TransformComponent, MeshComponent>();

		for (const auto& [rotationComp, transformComp, meshComp] : components)
		{
			if (!rotationComp || !transformComp || !meshComp)
			{
				LOG_ERROR("Null component found");
				continue;
			}

			Math::Matrix4x4 rotationMatrix = Math::Matrix4x4::GetRotationMatrix(rotationComp->axis, rotationComp->speed);
			Math::Matrix4x4 newMatrix = transformComp->transformMatrix * rotationMatrix;

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					transformComp->transformMatrix[i][j] = newMatrix[i][j];
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