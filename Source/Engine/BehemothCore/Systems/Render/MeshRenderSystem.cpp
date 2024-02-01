#include "pch.h"
#include "MeshRenderSystem.h"
#include "Core/Log.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Misc/CameraHelper.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Renderer/Renderer.h"
#include "Geometry/Primitives.h"
#include "Geometry/MeshLoader.h"
#include "Geometry/Mesh.h"
#include "Core/ResourceManager.h"
#include "Core/ThreadPool.h"


namespace Behemoth
{
	void MeshRenderSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		CameraComponent* mainCameraComp = CameraHelper::GetMainCamera(registry);
		TransformComponent* cameraTransformComp = CameraHelper::GetMainCameraTransform(registry);

		// Used for primitive indexes in renderer, allows for multi-threading without having to lock the renderers primitive container
		std::uint32_t renderSlotIndex = Renderer::GetInstance().GetCurrentPrimitiveCount();
		std::uint32_t primitivesToDraw = 0;

		BMath::Matrix4x4 viewProjMatrix = mainCameraComp->projMatrix * mainCameraComp->viewMatrix;

		auto components = registry.Get<MeshComponent, TransformComponent>();

		// Count total number of possible primitives to render so that renderer primitive container is only resized once
		// Will free unused space at the end
		for (const auto& [entityHandle, meshComp, transformComp] : components)
		{
			primitivesToDraw += meshComp->mesh.meshPrimitives.size();
		}

		ReserveResources(primitivesToDraw);

		for (const auto& [entityHandle, meshComp, transformComp] : components)
		{
			// Do not want to perform calculations if nothing is to be drawn
			if (!meshComp->isVisible)
			{
				continue;
			}

			// Check if the bounding volume is inside the frustum, if not skip rendering this primitive entirely 
			BoundingVolumeComponent* boundingVolumeComp = registry.GetComponent<BoundingVolumeComponent>(entityHandle);
			if (boundingVolumeComp && !IsBoundingVolumeInFrustrum( mainCameraComp, transformComp, boundingVolumeComp))
			{
				continue;
			}

			// Process primitive render calculations on worker thread
			ThreadPool::GetInstance().Enqueue(std::bind(
				&MeshRenderSystem::ProcessMesh, 
				this, 
				std::ref(meshComp->mesh),
				cameraTransformComp,
				transformComp->worldTransform,
				viewProjMatrix, 
				mainCameraComp->forwardVector,
				transformComp->isDirty,
				renderSlotIndex));

			// Increase the index for the next primitive
			renderSlotIndex += meshComp->mesh.meshPrimitives.size();

			// Can finally set it's transform to not dirty, this should be the only place that will set transforms isDirty to false
			transformComp->isDirty = false;
		}

		// Ensure that we have finished processing all primitives before freeing unused space and moving to lighting calculations
		ThreadPool::GetInstance().WaitForCompletion();

		//Free unused space in render's primitive container
		Renderer::GetInstance().FreePrimitiveResourceOverflow();
	}

	void MeshRenderSystem::ReserveResources(int numPrimitives)
	{
		Renderer::GetInstance().ReservePrimitives(numPrimitives);
	}

	bool MeshRenderSystem::CullBackFace(const BMath::Vector3& cameraLocation, const BMath::Vector3 forwardVec, const BMath::Vector4 primitiveVerts[])
	{

		BMath::Vector3 p1 = BMath::Vector3(primitiveVerts[0]) - cameraLocation;
		BMath::Vector3 p2 = BMath::Vector3(primitiveVerts[1]) - cameraLocation;
		/*		BMath::Vector3 p3 = BMath::Vector3(primitiveVerts[2]) - cameraLocation;*/

		if (BMath::Vector3::Dot(forwardVec, p1) < 0 || BMath::Vector3::Dot(forwardVec, p2) < 0 /*|| BMath::Vector3::Dot(forwardVec, p3) < 0*/)
		{
			return true;
		}

		BMath::Vector3 n1 = BMath::Vector3(BMath::Vector4::Cross(primitiveVerts[1] - primitiveVerts[0], primitiveVerts[2] - primitiveVerts[0]));
		BMath::Vector3 n2 = BMath::Vector3(BMath::Vector4::Cross(primitiveVerts[2] - primitiveVerts[1], primitiveVerts[0] - primitiveVerts[1]));
		/*		 BMath::Vector3 n3 = BMath::Vector3(BMath::Vector4::Cross(primitiveVerts[0] - primitiveVerts[1], primitiveVerts[1] - primitiveVerts[2]));*/

				// Back-face culling - if normals are not pointing towards camera cull the primitive
		return (BMath::Vector3::Dot(n1, p1) > 0 && BMath::Vector3::Dot(n2, p2) > 0 /*&& BMath::Vector3::Dot(n3, p3) > 0*/);
	}

	void MeshRenderSystem::AddPrimitiveToRenderer(Primitive& primitive, const int numVertices, const BMath::Vector4 vertex[], int renderSlotIndex)
	{
		primitive.SetSpriteVertices(numVertices, vertex);
		Renderer::GetInstance().AddPrimitive(&primitive, renderSlotIndex);
	}

	float MeshRenderSystem::GetPrimitiveDepth(const int numVertices, const BMath::Vector4 vertex[])
	{
		float depth = 0.0f;
		for (int j = 0; j < numVertices; j++)
		{
			depth += vertex[j].z;
		}
		return depth / numVertices;
	}

	void MeshRenderSystem::ProcessMesh(
		Mesh& mesh,
		TransformComponent* cameraTransformComp,
		const BMath::Matrix4x4& transformMatrix,
		const BMath::Matrix4x4& viewProjMatrix,
		const BMath::Vector3& cameraWorldFwdVec,
		bool isDirty,
		int renderSlotIndex)
	{
		const MeshData& meshData = mesh.meshData;

		// Only need to load this if the transform is dirty otherwise the world space locations of the model are still correct from laster render pass
		// need to find a good way to only include if is dirty
		const std::vector<VertexData>& vertices = ResourceManager::GetInstance().GetMeshVertices(meshData.modelFileName);

		int numVertices = 3;
		for (int i = 0, vertexIndex = 0; i < meshData.totalPrimitives; i++)
		{
			// If we have finished rendering all the triangle primitives move to quads
			if (vertexIndex >= meshData.triangleVertexCount)
			{
				numVertices = 4;
			}

			Primitive& primitive = mesh.meshPrimitives[i];

			// Only need to update the vertices if matrix dirty, otherwise reuse the previous pass' world space vertex data
			if (isDirty)
			{
				for (int j = 0; j < numVertices; j++)
				{
					primitive.vertices[j] = transformMatrix * BMath::Vector4(vertices[vertexIndex++].position, 1.0f);
				}
			}
			else
			{
				vertexIndex += numVertices;
			}

			// Check if primitive face is in direction of camera or if it can be culled
			if (CullBackFace(cameraTransformComp->worldPosition, cameraWorldFwdVec, primitive.vertices))
			{
				continue;
			}

			// Store what will become the NDC coordinates in a separate container
			// This is so we can use the primitives world space vertex info later in places such as lighting
			BMath::Vector4 renderVerts[4];
			memcpy(renderVerts, primitive.vertices, sizeof(BMath::Vector4) * 4);

			// Multiplies the primitive by the view projection matrix, gets the depth to be used for draw order
			primitive.depth = ProcessVertex(viewProjMatrix, renderVerts, numVertices);
 
			// Ensure primitive is inside the view frustum
			if (!IsPrimitiveWithinFrustrum(numVertices, renderVerts))
			{
				continue;
			}

			// Finally add the primitive to the renderer to be drawn
			AddPrimitiveToRenderer(primitive, numVertices, renderVerts, renderSlotIndex);
			renderSlotIndex++;
		}
	}
}