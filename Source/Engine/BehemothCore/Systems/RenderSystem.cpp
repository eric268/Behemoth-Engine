#include "RenderSystem.h"
#include "Misc/Log.h"
#include "Components/Components.h"

namespace Behemoth
{
	void RenderSystem::Run(ECS::Registry& registry)
	{
		auto components = registry.Get<MeshComponent, TransformComponent>();
		auto cameraComponents = registry.Get<CameraComponent, TransformComponent, FrustrumComponent>();

		FrustrumComponent* mainCameraFrustrumComponent = nullptr;
		CameraComponent* mainCamera = nullptr;
		TransformComponent* mainCameraTransform = nullptr;

		for (const auto& [cameraComp, transformComp, frustrumComp] : cameraComponents)
		{
			if (cameraComp->isMain)
			{
				mainCamera = cameraComp;
				mainCameraTransform = transformComp;
				mainCameraFrustrumComponent = frustrumComp;
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

			// Do not want to perform calculations if nothing is to be drawn
			if (!meshComp->isVisible && !meshComp->drawWireMesh)
				continue;

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

				// If the object is not within view no need to calculate anything further or draw anything for the entire object
				if (!IsInFrustrum(mainCamera, mainCameraFrustrumComponent, transformComp, 2.5f))
				{
					break;
				}

				// If face is not visible no need for further calculations or drawing of this primitive
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
				
				if (meshComp->drawWireMesh)
					meshComp->mesh.meshPrimitives[i].DrawWireMesh(type);

				if (meshComp->isVisible)
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

	bool RenderSystem::IsInFrustrum(const CameraComponent* cameraComponent, const FrustrumComponent* frustrumComp, const TransformComponent* transformComp, const float boundingRadius)
	{
		for (const auto& p : frustrumComp->worldSpacePlanes)
		{
			float distance = Math::Vector3::Dot(p.normal, transformComp->position) + p.distance;
			if (distance < -boundingRadius - 1e-4)
				return false;
		}

		return true;
	}

}