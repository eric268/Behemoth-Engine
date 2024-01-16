#include "pch.h"
#include "SkySphereSystem.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/RenderComponents.h"
#include "Components/Components.h"
#include "Misc/CameraHelper.h"
#include "Application/ResourceManager.h"
#include "Geometry/Mesh.h"

namespace Behemoth
{
	void SkySphereSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		CameraComponent* cameraComp = CameraHelper::GetMainCamera(registry);
		TransformComponent* cameraTransform = CameraHelper::GetMainCameraTransform(registry);

		for (const auto& [entity, transformComp, skySphereComp] : registry.Get<TransformComponent, SkySphereComponent>())
		{
			if (!skySphereComp->isInitalized)
			{
				InitalizeSphere(skySphereComp);
			}
			FollowCamera(transformComp, cameraTransform->worldPosition);

			BMath::Matrix4x4f viewProjMatrix = cameraComp->projMatrix * cameraComp->viewMatrix;
			ProcessSphere(transformComp, skySphereComp, viewProjMatrix);
		}
	}

	void SkySphereSystem::InitalizeSphere(SkySphereComponent* skySphereComponent)
	{
		const std::vector<VertexData>& vertexData = ResourceManager::GetInstance().GetMeshVerticies(skySphereComponent->mesh.meshData.modelFileName);
		const MeshData& meshData = ResourceManager::GetInstance().GetMeshData(skySphereComponent->mesh.meshData.modelFileName);
		skySphereComponent->mesh.GenerateMesh(meshData, vertexData);
		skySphereComponent->verticies.resize(vertexData.size());

		for (int i = 0; i < vertexData.size(); i++)
		{
			skySphereComponent->verticies[i] = vertexData[i].vertex;
		}

		skySphereComponent->isInitalized = true;
	}
	void SkySphereSystem::ProcessSphere(TransformComponent* transformComp, SkySphereComponent* skySphereComponent, const BMath::Matrix4x4f& viewProjMatrix)
	{
		const MeshData& meshData = skySphereComponent->mesh.meshData;

		ReserveResources(meshData.totalPrimitives);

		int numVerticies = 3;
		for (int i = 0, vertexIndex = 0; i < meshData.totalPrimitives; i++)
		{
			if (vertexIndex >= meshData.triangleVertexCount)
			{
				numVerticies = 4;
			}

			Primitive& primitive = skySphereComponent->mesh.meshPrimitives[i];

			// Only need to update the verticies if matrix dirty
			if (transformComp->isDirty)
			{
				for (int j = 0; j < numVerticies; j++)
				{
					primitive.verticies[j] = transformComp->worldTransform * BMath::Vector4(skySphereComponent->verticies[vertexIndex], 1.0f);
					vertexIndex++;
				}
			}
			else
			{
				vertexIndex += numVerticies;
			}

			BMath::Vector4 renderVerts[4];
			memcpy(renderVerts, primitive.verticies, sizeof(BMath::Vector4) * 4);

			ProcessVertex(viewProjMatrix, renderVerts, numVerticies);

			if (!IsPrimitiveWithinFrustrum(numVerticies, renderVerts))
			{
				continue;
			}

			// Want to always render this first to be in front of all other primtives
			primitive.depth = std::numeric_limits<float>::max();
			AddPrimitiveToRenderer(primitive, numVerticies, renderVerts);
		}
	}

	void SkySphereSystem::FollowCamera(TransformComponent* transformComp, const BMath::Vector3& cameraPosition)
	{
		transformComp->worldPosition = cameraPosition;
		transformComp->worldTransform._41 = cameraPosition.x;
		transformComp->worldTransform._42 = cameraPosition.y;
		transformComp->worldTransform._43 = cameraPosition.z;
		transformComp->isDirty = true;
	}
}