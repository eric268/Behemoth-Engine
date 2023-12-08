#include "RenderSystem.h"
#include "Components/Components.h"
#include "Misc/Log.h"

namespace Behemoth
{
	void RenderSystem::Run(ECS::Registry& registry)
	{
		auto components = registry.Get<MeshComponent, TransformComponent>();
		auto cameraComponents = registry.Get<CameraComponent, TransformComponent>();

		CameraComponent* mainCamera = nullptr;
		TransformComponent* mainCameraTransform = nullptr;

		for (const auto& [cameraComp, transformComp] : cameraComponents)
		{
			if (cameraComp->isMain)
			{
				mainCamera = cameraComp;
				mainCameraTransform = transformComp;
				break;
			}
		}

		if (!mainCamera || !mainCameraTransform)
		{
			LOG_ERROR("Main camera not found");
			return;
		}

		for (const auto& [meshComp, transformComp] : components)
		{
			if (!meshComp || !transformComp)
			{
				LOG_ERROR("Null component found");
				continue;
			}

			// ** Order of multiplication matters here **
			Math::Matrix4x4 viewProjMatrix = mainCamera->perspectiveMatrix * mainCamera->viewMatrix;

			std::size_t size = meshComp->mesh.meshPrimitives.size();
			for (int i = 0; i < size; i++)
			{
				Math::Vector4 vertex[4];

				PrimitiveType type = meshComp->mesh.meshPrimitives[i].primitiveType;

				for (int j = 0; j < static_cast<int>(type); j++)
				{
					vertex[j] = Math::Vector4(meshComp->mesh.meshPrimitives[i].verticies[j], 1.0f);
					vertex[j] = vertex[j] * transformComp->transformMatrix;
				}

				if (CullBackFace(mainCameraTransform->position, vertex))
				{
					continue;
				}

				for (int j = 0; j < static_cast<int>(type); j++)
				{
					vertex[j] = vertex[j] * viewProjMatrix;

					assert(vertex[j].w != 0.0f);
					vertex[j] = vertex[j] / vertex[j].w;
				}

				meshComp->mesh.meshPrimitives[i].SetSpriteVerticies(type, vertex, meshComp->mesh.meshPrimitives[i].uv);
				// meshComp->mesh.meshPrimitives[i].DrawWireMesh(type);
				meshComp->mesh.meshPrimitives[i].Draw();
			}
		}
	}

	bool RenderSystem::CullBackFace(const Math::Vector3& cameraLocation, const Math::Vector4 primitiveVerts[])
	{

		const Math::Vector3 normal = Math::Vector3(Math::Vector4::Cross(primitiveVerts[1] - primitiveVerts[0], primitiveVerts[2] - primitiveVerts[0]));
		const Math::Vector3 cam = cameraLocation - Math::Vector3(primitiveVerts[0]);
		return (Math::Vector3::Dot(normal, cam)) <= 0;
	}
}