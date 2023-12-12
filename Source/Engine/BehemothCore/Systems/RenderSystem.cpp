#include "RenderSystem.h"
#include "Misc/Log.h"
#include "Components/Components.h"
#include "ECS/Entity.h"
#include "Render/Renderer.h"

namespace Behemoth
{
	void RenderSystem::Run(ECS::Registry& registry)
	{
		auto components = registry.Get<MeshComponent, TransformComponent>();
		auto cameraComponents = registry.Get<CameraComponent, TransformComponent, FrustrumComponent>();

		FrustrumComponent* mainCameraFrustrumComponent = nullptr;
		CameraComponent* mainCamera = nullptr;
		TransformComponent* mainCameraTransform = nullptr;

		for (const auto& [entity, cameraComp, transformComp, frustrumComp] : cameraComponents)
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

		// ** Order of multiplication matters here **
		Math::Matrix4x4 viewProjMatrix = mainCamera->perspectiveMatrix * mainCamera->viewMatrix;

		for (const auto& [entity, meshComp, transformComp] : components)
		{
			// Do not want to perform calculations if nothing is to be drawn
			if (!meshComp->isVisible && !meshComp->drawWireMesh)
				continue;

			BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(entity);
			if (boundingVolume && !IsBoundingVolumeInFrustrum(mainCamera, mainCameraFrustrumComponent, transformComp, boundingVolume->volumeRadius))
			{
				continue;
			}

			DrawMesh(meshComp->mesh, meshComp->isVisible, meshComp->drawWireMesh, mainCameraTransform->position, transformComp->transformMatrix, viewProjMatrix);

#ifdef DEBUG
			if (boundingVolume && boundingVolume->drawBoundingVolume)
				DrawBoundingVolume(boundingVolume->mesh, boundingVolume->volumeRadius, mainCameraTransform->position, transformComp->transformMatrix, viewProjMatrix);
#endif
		}
	}

	bool RenderSystem::CullBackFace(const Math::Vector3& cameraLocation, const Math::Vector4 primitiveVerts[])
	{
		Math::Vector3 normal = Math::Vector3(Math::Vector4::Cross(primitiveVerts[1] - primitiveVerts[0], primitiveVerts[2] - primitiveVerts[0]));
		const Math::Vector3 cam = cameraLocation - Math::Vector3(primitiveVerts[0]);
		return (Math::Vector3::Dot(normal, cam)) <= 0;
	}

	bool  RenderSystem::CullQuadBackFace(const Math::Vector3& cameraLocation, const Math::Vector4 primitiveVerts[])
	{
		Math::Vector3 edge1 = Math::Vector3(primitiveVerts[1] - primitiveVerts[0]);
		Math::Vector3 edge2 = Math::Vector3(primitiveVerts[2] - primitiveVerts[0]);
		Math::Vector3 edge3 = Math::Vector3(primitiveVerts[3] - primitiveVerts[0]);

		Math::Vector3 normal1 = Math::Vector3::Cross(edge1, edge2).Normalize();
		Math::Vector3 normal2 = Math::Vector3::Cross(edge1, edge3).Normalize();

		Math::Vector3 normal = (normal1 + normal2) * 0.5f;

		const Math::Vector3 cam = cameraLocation - Math::Vector3(primitiveVerts[0]);
		return Math::Vector3::Dot(normal, cam) < 1e-5;

	}

	bool RenderSystem::IsBoundingVolumeInFrustrum(const CameraComponent* cameraComponent, const FrustrumComponent* frustrumComp, const TransformComponent* transformComp, const float boundingRadius)
	{
		for (const auto& p : frustrumComp->worldSpacePlanes)
		{
			float distance = Math::Vector3::Dot(p.normal, transformComp->position) + p.distance;
			if (distance < -boundingRadius - 1e-4)
				return false;
		}

		return true;
	}

	void RenderSystem::DrawMesh(Mesh& mesh, bool isVisible, bool drawWireFrame, const Math::Vector3 cameraPosition, const Math::Matrix4x4& meshTransform, const Math::Matrix4x4& viewProjMatrix)
	{
		Renderer::GetInstance().ReservePrimitives(mesh.totalPrimitives);
		for (int i = 0; i < mesh.totalPrimitives; i++)
		{
			Math::Vector4 vertex[4];

			PrimitiveType type = mesh.meshPrimitives[i].primitiveType;
			int numVerticies = static_cast<int>(type);

			for (int j = 0; j < numVerticies; j++)
			{
				vertex[j] = Math::Vector4(mesh.meshPrimitives[i].verticies[j], 1.0f);
				vertex[j] = vertex[j] * meshTransform;
			}

			if (CullBackFace(cameraPosition, vertex))
			{
				continue;
			}

			float depth = 0.0f;
			for (int j = 0; j < numVerticies; j++)
			{
				vertex[j] = vertex[j] * viewProjMatrix;
				assert(vertex[j].w != 0.0f);
				vertex[j] = vertex[j] / vertex[j].w;
				depth += vertex[j].z;
			}
			assert(numVerticies != 0);
			
			// No need to call draw on primitive that is not visible in view frustum 
			if (!IsPrimitiveWithinFrustrum(numVerticies, vertex))
			{
				continue;
			}
		

			mesh.meshPrimitives[i].SetSpriteVerticies(type, vertex);

			if (isVisible)
			{
				// mesh.meshPrimitives[i].Draw();
				mesh.meshPrimitives[i].depth = depth / numVerticies;
				Renderer::GetInstance().AddPrimitive(&mesh.meshPrimitives[i]);
			}

			if (drawWireFrame)
				mesh.meshPrimitives[i].DrawWireMesh(type);
		}
	}

	void RenderSystem::DrawBoundingVolume(Mesh& mesh, const float radius, const Math::Vector3& cameraPosition, const Math::Matrix4x4& meshTransform, const Math::Matrix4x4& viewProjMatrix)
	{
		Math::Matrix4x4 boundingMatrix{};

		for (int i = 0; i < 3; i++)
		{
			boundingMatrix[i][i] = radius;
			boundingMatrix[3][i] = meshTransform[3][i];
		}

		DrawMesh(mesh, false, true, cameraPosition, boundingMatrix, viewProjMatrix);
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
				primitiveVerts[i].z <= 0)
				numVerticiesOutsideFrustrum++;
		}
		
		return numVerticiesOutsideFrustrum != numVerticies;
	}
}