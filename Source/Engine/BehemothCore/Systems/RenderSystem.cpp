#include "pch.h"
#include "RenderSystem.h"
#include "Misc/Log.h"
#include "Misc/CameraHelper.h"
#include "Components/Components.h"
#include "ECS/Entity.h"
#include "Render/Renderer.h"
#include "Render/MeshLoader.h"
#include "Application/ResourceManager.h"

/*
* - Need to create a class that will hold a lot of the rendering information, such as vertex position, recieve lighting, diffuse specular shinniess, alpha etc.
* - Then when iterating through a mesh to determine primitives to be drawn start with a vector that contains the max number of primitives
*  - Then shink to fit only primitives that are actually drawn
* - Then reserve this space in the render queue singleton and copy this data over ... 
* - Profile if this is faster or slow then reversing the entire primitive in the render queue for all primitives then shrinking at the end
* - Then when finally drawing in the renderer set the actual primitive reference verticies to that of the rendering primitive
*/

namespace Behemoth
{
	void RenderSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<MeshComponent, TransformComponent>();
		auto cameraComponents = registry.Get<CameraComponent, TransformComponent>();

		CameraComponent* mainCamera = CameraHelper::GetMainCamera(registry);
		Math::Vector3 mainCameraPosition = CameraHelper::GetMainCameraPostition(registry);

		std::sort(components.begin(), components.end(),
			[&](std::tuple<ECS::Entity, MeshComponent*, TransformComponent*> tuple1, std::tuple<ECS::Entity, MeshComponent*, TransformComponent*> tuple2)
			{
				return std::get<1>(tuple1)->mesh.meshPrimitives.size() < std::get<1>(tuple2)->mesh.meshPrimitives.size();
			});

		// ** Order of multiplication matters here **
		Math::Matrix4x4 viewProjMatrix = mainCamera->perspectiveMatrix * mainCamera->viewMatrix;

		for (const auto& [entity, meshComp, transformComp] : components)
		{
			// Do not want to perform calculations if nothing is to be drawn
			if (!meshComp->isVisible && !meshComp->drawWireMesh)
				continue;

			BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(entity);

			if (boundingVolume && !IsBoundingVolumeInFrustrum(mainCamera, transformComp, boundingVolume->volumeRadius))
			{
				continue;
			}

#ifdef DEBUG
			if (boundingVolume && boundingVolume->drawBoundingVolume)
				DrawBoundingVolume(boundingVolume->mesh, boundingVolume->volumeRadius, mainCameraPosition, transformComp->transformMatrix, viewProjMatrix, transformComp->dirty);
#endif

			ProcessMesh(meshComp->mesh, meshComp->isVisible, meshComp->drawWireMesh, mainCameraPosition, transformComp->transformMatrix, viewProjMatrix, transformComp->dirty);
			transformComp->dirty = false;
		}

		Renderer::GetInstance().FreeResourceOverflow();
	}


	void RenderSystem::ProcessMesh(Mesh& mesh, bool isVisible, bool drawWireMesh, const Math::Vector3 cameraPosition, const Math::Matrix4x4& transformMatrix, const Math::Matrix4x4& viewProjMatrix, bool isDirty)
	{
		const MeshData& meshData = mesh.meshData;

		ReserveResources(meshData.totalPrimitives, drawWireMesh);
		if (isDirty && cachedMeshName != meshData.modelFileName)
		{
			cachedMeshName = meshData.modelFileName;
			cachedVerticies = ResourceManager::GetInstance().GetMeshVerticies(meshData.modelFileName);
		}

		int numVerticies = 3;
		for (int i = 0, vertexIndex = 0; i < meshData.totalPrimitives; i++)
		{
			if (vertexIndex >= meshData.triangleVertexCount)
				numVerticies = 4;

			Primitives& primitive = mesh.meshPrimitives[i];

			// Only need to update the verticies if matrix dirty
			if (isDirty)
			{
				for (int j = 0; j < numVerticies; j++)
				{
					primitive.verticies[j] = transformMatrix * Math::Vector4(cachedVerticies[vertexIndex].vertex, 1.0f);
					vertexIndex++;
				}
			}

			

			bool cullPrimitive = CullBackFace(cameraPosition, primitive.verticies);
			if (cullPrimitive && !drawWireMesh)
			{
				continue;
			}

			Math::Vector4 renderVerts[4];
			memcpy(renderVerts, primitive.verticies, sizeof(Math::Vector4) * 4);

			ProcessVertex(viewProjMatrix, renderVerts, numVerticies);

			if (!IsPrimitiveWithinFrustrum(numVerticies, renderVerts))
			{
				continue;
			}

			if (drawWireMesh)
			{
				AddWireMeshToRenderer(numVerticies, renderVerts);
			}

			if (!cullPrimitive && isVisible)
			{
				AddPrimitiveToRenderer(primitive, numVerticies, renderVerts);
			}
		}
	}

	bool RenderSystem::CullBackFace(const Math::Vector3& cameraLocation, const Math::Vector4 primitiveVerts[])
	{
		Math::Vector3 normal = Math::Vector3(Math::Vector4::Cross(primitiveVerts[1] - primitiveVerts[0], primitiveVerts[2] - primitiveVerts[0]));
		const Math::Vector3 cam = cameraLocation - Math::Vector3(primitiveVerts[0]);
		return (Math::Vector3::Dot(normal, cam)) <= 0;
	}

	bool RenderSystem::IsBoundingVolumeInFrustrum(const CameraComponent* cameraComponent, const TransformComponent* boundingTransformComp, const float boundingRadius)
	{
		for (const auto& p : cameraComponent->worldSpaceFrustum)
		{
 			float distance = Math::Vector3::Dot(p.normal, boundingTransformComp->position) - p.distance;
			if (distance < -boundingRadius)
				return false;
		}

		return true;
	}

	void RenderSystem::DrawBoundingVolume(Mesh& mesh, const float radius, const Math::Vector3& cameraPosition, const Math::Matrix4x4& transformMatrix, const Math::Matrix4x4& viewProjMatrix, bool isDirty)
	{
		Math::Matrix4x4 scalingMatrix = Math::Matrix4x4::Identity();

		Math::Matrix4x4 boundingMatrix ;

		for (int i = 0; i < 3; i++)
		{
			scalingMatrix.data[i][i] = radius;
// 			boundingMatrix.data[i][i] = radius;
// 			boundingMatrix.data[i][3] = transformMatrix.data[i][3];
		}
		boundingMatrix = transformMatrix * scalingMatrix;

		ProcessMesh(mesh, false, true, cameraPosition, boundingMatrix, viewProjMatrix, isDirty);
	}

	bool RenderSystem::IsPrimitiveWithinFrustrum(const int numVerticies, Math::Vector4 primitiveVerts[])
	{
		// Only want to cull primitives that are entirely outside of view frustum since OpenGL render pipeline will clip
		// the quads if portions are outside the frustum. Me doing that, potentially creating temporary primitives will
		// just increase draw calls with is counter intuitive 
		int numVerticiesOutsideFrustrum = 0;
		for (int i = 0; i < numVerticies; i++)
		{
			if (primitiveVerts[i].x < -primitiveVerts[i].w || 
				primitiveVerts[i].x >  primitiveVerts[i].w ||
				primitiveVerts[i].y < -primitiveVerts[i].w ||
				primitiveVerts[i].y >  primitiveVerts[i].w || 
				primitiveVerts[i].z >  primitiveVerts[i].w ||
				primitiveVerts[i].z <= 0.0f)
				numVerticiesOutsideFrustrum++;
		}
		
		return numVerticiesOutsideFrustrum != numVerticies;
	}

	void RenderSystem::AddWireMeshToRenderer(const int numVerticies, const Math::Vector4 verticies[])
	{
		for (int i = 0, j = i + 1; i < numVerticies; i++, j++)
		{
			j = (j >= numVerticies) ? 0 : j;
			Renderer::GetInstance().AddLine(Math::Vector4(verticies[i].x, verticies[i].y, verticies[j].x, verticies[j].y));
		}
	}

	void RenderSystem::AddPrimitiveToRenderer(Primitives& primitive, const int numVerticies, const Math::Vector4 vertex[])
	{
		primitive.depth = GetPrimitiveDepth(numVerticies, vertex);
		primitive.SetSpriteVerticies(numVerticies, vertex);
		Renderer::GetInstance().AddPrimitive(&primitive);
	}


	void RenderSystem::ReserveResources(int numPrimitives, bool drawWireFrame)
	{
		Renderer::GetInstance().ReservePrimitives(numPrimitives);

		if (drawWireFrame) 
		{
			Renderer::GetInstance().ReserveLines(numPrimitives * 4);
		}
	}

	void RenderSystem::TransformVertex(const Primitives& primitive, const Math::Matrix4x4& transformMatrix, Math::Vector4 vertex[], const int numVerticies)
	{
		for (int j = 0; j < numVerticies; j++)
		{
			vertex[j] = transformMatrix * primitive.verticies[j];
		}
	}

	void RenderSystem::ProcessVertex(const Math::Matrix4x4& viewProjMatrix, Math::Vector4 vertex[], int numVerticies)
	{
		for (int j = 0; j < numVerticies; j++)
		{
			vertex[j] = viewProjMatrix * vertex[j];

			if (vertex[j].w == 0.0f)
			{
				vertex[j].w = 0.01f;
			}

			float w = 1.0f / vertex[j].w;

			vertex[j] *= w;
		}
	}

	float RenderSystem::GetPrimitiveDepth(const int numVerticies, const Math::Vector4 vertex[])
	{
		float depth = 0.0f;
		for (int j = 0; j < numVerticies; j++)
		{
			depth += vertex[j].z;
		}
		return depth / numVerticies;
	}
}