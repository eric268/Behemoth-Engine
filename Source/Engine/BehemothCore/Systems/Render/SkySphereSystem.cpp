#include "pch.h"
#include "SkySphereSystem.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/RenderComponents.h"
#include "Components/Components.h"
#include "Misc/CameraHelper.h"
#include "Core/ResourceManager.h"
#include "Geometry/Mesh.h"
#include "Renderer/Renderer.h"
#include "Core/Stopwatch.h"

namespace Behemoth
{
	void SkySphereSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		CameraComponent* cameraComp = CameraHelper::GetMainCamera(registry);
		TransformComponent* cameraTransform = CameraHelper::GetMainCameraTransform(registry);

		std::uint32_t renderSlotIndex = Renderer::GetInstance().GetCurrentPrimitiveCount();

		for (const auto& [
			entity, 
				transformComp,
				skySphereComp] : registry.Get<TransformComponent, SkySphereComponent>())
		{
			if (!skySphereComp->isInitalized)
			{
				InitalizeSphere(skySphereComp);
			}
			 FollowCamera(transformComp, cameraTransform->worldPosition);

			BMath::BMatrix4x4 viewProjMatrix = cameraComp->projMatrix * cameraComp->viewMatrix;

			ProcessSphere(transformComp, skySphereComp, cameraTransform, viewProjMatrix, renderSlotIndex);
			renderSlotIndex += skySphereComp->mesh.meshPrimitives.size();
		}

		Renderer::GetInstance().FreePrimitiveResourceOverflow();
	}

	// Since this primitive should only exist once in the scene it is fine to store the verticies directly inside the component
	// Otherwise the models verticies are stored in the resource manager and loaded for rendering
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

		// Want all of the "sky sphere" primitives to be drawn first since they should always be considered the furthest possible
		for (auto& primitive : skySphereComponent->mesh.meshPrimitives)
		{
			primitive.depth = std::numeric_limits<float>::max();
		}

		skySphereComponent->isInitalized = true;
	}

	// Should be very similar to the mesh render class except their is no back face culling since we will be 
	void SkySphereSystem::ProcessSphere(
		TransformComponent* transformComp, 
		SkySphereComponent* skySphereComponent,
		TransformComponent* cameraTransform,
		const BMath::BMatrix4x4& viewProjMatrix, 
		int renderSlotIndex)
	{
		const MeshData& meshData = skySphereComponent->mesh.meshData;


		ReserveResources(meshData.totalPrimitives);
		int primitiveIndex = 0;
		int numVerticies = 3;

		for (int i = 0, vertexIndex = 0; i < meshData.totalPrimitives; i++)
		{
			if (vertexIndex >= meshData.triangleVertexCount)
			{
				numVerticies = 4;
				int completeTriangles = meshData.triangleVertexCount / 3;
				int remainingVerticies = vertexIndex - meshData.triangleVertexCount;
				int completeQuads = remainingVerticies / 4;
				primitiveIndex = completeTriangles + completeQuads;
			}
			else
			{
				primitiveIndex = vertexIndex / 3;
			}

			Primitive& primitive = skySphereComponent->mesh.meshPrimitives[i];

			// Only need to update the verticies if matrix dirty
			if (transformComp->isDirty)
			{
				for (int j = 0; j < numVerticies; j++)
				{
					primitive.verticies[j] = transformComp->worldTransform * BMath::Vector4(skySphereComponent->verticies[vertexIndex++], 1.0f);
				}
			}
			else
			{
				vertexIndex += numVerticies;
			}

			// We can invert this function result since we are inside the sphere, so primitives pointing in the same direction as the 
			// camera (back face) should be renderer, and primitives pointing back at the camera should be culled (front faces)
			if (!CullBackFace(cameraTransform->worldPosition, cameraTransform->forwardVector, primitive.verticies))
			{
				continue;
			}

			BMath::Vector4 renderVerts[4];
			memcpy(renderVerts, primitive.verticies, sizeof(BMath::Vector4) * 4);

			ProcessVertex(viewProjMatrix, renderVerts, numVerticies);

			if (!IsPrimitiveWithinFrustrum(numVerticies, renderVerts))
			{
				continue;
			}

			AddPrimitiveToRenderer(primitive, numVerticies, renderVerts, primitiveIndex);
			primitiveIndex++;
		}
	}

	// Want to keep sky sphere at cameras origin, but not include rotation so it appears we are looking around the world
	void SkySphereSystem::FollowCamera(TransformComponent* transformComp, const BMath::Vector3& cameraPosition)
	{
		transformComp->worldPosition = cameraPosition;
		transformComp->worldTransform._41 = cameraPosition.x;
		transformComp->worldTransform._42 = cameraPosition.y;
		transformComp->worldTransform._43 = cameraPosition.z;
		transformComp->isDirty = true;
	}
}