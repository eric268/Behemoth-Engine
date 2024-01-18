#include "pch.h"
#include "MeshRenderSystem.h"
#include "Core/Log.h"
#include "Misc/CameraHelper.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "ECS/Entity.h"
#include "Renderer/Renderer.h"
#include "Geometry/MeshLoader.h"
#include "Core/ResourceManager.h"
#include "Core/ThreadPool.h"

namespace Behemoth
{
	void MeshRenderSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<MeshComponent, TransformComponent>();

		CameraComponent* mainCamera = CameraHelper::GetMainCamera(registry);
		TransformComponent* cameraTransformComp = CameraHelper::GetMainCameraTransform(registry);

		std::sort(components.begin(), components.end(),
			[&](std::tuple<ECS::Entity, MeshComponent*, TransformComponent*> tuple1, std::tuple<ECS::Entity, MeshComponent*, TransformComponent*> tuple2)
			{
				return std::get<1>(tuple1)->mesh.meshPrimitives.size() < std::get<1>(tuple2)->mesh.meshPrimitives.size();
			});

		std::uint32_t renderSlotIndex = Renderer::GetInstance().GetCurrentPrimitiveCount();
		std::uint32_t primitivesToDraw = 0;

		// ** Order of multiplication matters here **
		BMath::Matrix4x4 viewProjMatrix = mainCamera->projMatrix * mainCamera->viewMatrix;

		for (const auto& [entity, meshComp, transformComp] : components)
		{
			primitivesToDraw += meshComp->mesh.meshPrimitives.size();
		}

		ReserveResources(primitivesToDraw);

		for (const auto& [entity, meshComp, transformComp] : components)
		{
			// Do not want to perform calculations if nothing is to be drawn
			if (!meshComp->isVisible)
			{
				continue;
			}

			BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(entity);
			if (boundingVolume && !IsBoundingVolumeInFrustrum( mainCamera, transformComp, boundingVolume))
			{
				continue;
			}


			ThreadPool::GetInstance().Enqueue(std::bind(&MeshRenderSystem::ProcessMesh, this, std::ref(meshComp->mesh), cameraTransformComp, transformComp->worldTransform, viewProjMatrix, transformComp->isDirty, renderSlotIndex));
			renderSlotIndex += meshComp->mesh.meshPrimitives.size();
			transformComp->isDirty = false;
		}

		ThreadPool::GetInstance().WaitForCompletion();
		Renderer::GetInstance().FreeResourceOverflow();
	}


	void MeshRenderSystem::ProcessMesh(Mesh& mesh, TransformComponent* cameraTransform, const BMath::Matrix4x4f& transformMatrix, const BMath::Matrix4x4f& viewProjMatrix, bool isDirty, int renderSlotIndex)
	{
		const MeshData& meshData = mesh.meshData;

		// Only need to load this if the transform is dirty otherwise the world space locations of the model are still correct from laster render pass
		// need to find a good way to only include if is dirty
		const std::vector<VertexData>& verticies = ResourceManager::GetInstance().GetMeshVerticies(meshData.modelFileName);

		int numVerticies = 3;
		for (int i = 0, vertexIndex = 0; i < meshData.totalPrimitives; i++)
		{
			if (vertexIndex >= meshData.triangleVertexCount)
			{
				numVerticies = 4;
			}

			Primitive& primitive = mesh.meshPrimitives[i];

			// Only need to update the verticies if matrix dirty, otherwise reuse the previous pass' world space vertex data
			if (isDirty)
			{
				for (int j = 0; j < numVerticies; j++)
				{
					primitive.verticies[j] = transformMatrix * BMath::Vector4(verticies[vertexIndex++].vertex, 1.0f);
				}
			}
			else
			{
				vertexIndex += numVerticies;
			}

			if (CullBackFace(cameraTransform->worldPosition, cameraTransform->forwardVector, primitive.verticies))
			{
				continue;
			}

			BMath::Vector4 renderVerts[4];
			memcpy(renderVerts, primitive.verticies, sizeof(BMath::Vector4) * 4);

			primitive.depth = ProcessVertex(viewProjMatrix, renderVerts, numVerticies);
 
			if (!IsPrimitiveWithinFrustrum(numVerticies, renderVerts))
			{
				continue;
			}

			AddPrimitiveToRenderer(primitive, numVerticies, renderVerts, renderSlotIndex);
			renderSlotIndex++;
		}
	}

	bool MeshRenderSystem::CullBackFace(const BMath::Vector3& cameraLocation, const BMath::Vector3 forwardVec, const BMath::Vector4 primitiveVerts[])
	{

		BMath::Vector3 p1 = BMath::Vector3(primitiveVerts[0]) - cameraLocation;
		BMath::Vector3 p2 = BMath::Vector3(primitiveVerts[1]) - cameraLocation;
		BMath::Vector3 p3 = BMath::Vector3(primitiveVerts[2]) - cameraLocation;

		if (BMath::Vector3::Dot(forwardVec, p1) < 0 || BMath::Vector3::Dot(forwardVec, p2) < 0 /*|| BMath::Vector3::Dot(forwardVec, p3) < 0*/)
		{
			return true;
		}

		 BMath::Vector3 n1 = BMath::Vector3(BMath::Vector4::Cross(primitiveVerts[1] - primitiveVerts[0], primitiveVerts[2] - primitiveVerts[0]));
		 BMath::Vector3 n2 = BMath::Vector3(BMath::Vector4::Cross(primitiveVerts[2] - primitiveVerts[1], primitiveVerts[0] - primitiveVerts[1]));
		 BMath::Vector3 n3 = BMath::Vector3(BMath::Vector4::Cross(primitiveVerts[0] - primitiveVerts[1], primitiveVerts[1] - primitiveVerts[2]));

		// Back-face culling - if normals are not pointing towards camera cull the primitive
		return (BMath::Vector3::Dot(n1, p1) > 0 && BMath::Vector3::Dot(n2, p2) > 0 && BMath::Vector3::Dot(n3, p3) > 0);
	}


	void MeshRenderSystem::AddPrimitiveToRenderer(Primitive& primitive, const int numVerticies, const BMath::Vector4 vertex[], int renderSlotIndex)
	{
		primitive.SetSpriteVerticies(numVerticies, vertex);
		Renderer::GetInstance().AddPrimitive(&primitive, renderSlotIndex);
	}


	void MeshRenderSystem::ReserveResources(int numPrimitives)
	{
		Renderer::GetInstance().ReservePrimitives(numPrimitives);
	}

	float MeshRenderSystem::GetPrimitiveDepth(const int numVerticies, const BMath::Vector4 vertex[])
	{
		float depth = 0.0f;
		for (int j = 0; j < numVerticies; j++)
		{
			depth += vertex[j].z;
		}
		return depth / numVerticies;
	}
}