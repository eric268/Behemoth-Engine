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
		Math::Vector3 mainCameraPosition{};

		for (const auto& [entity, cameraComp, transformComp, frustrumComp] : cameraComponents)
		{
			if (cameraComp->isMain)
			{
				mainCamera = cameraComp;
				mainCameraPosition = transformComp->position;
				mainCameraFrustrumComponent = frustrumComp;
				break;
			}
		}

		if (!mainCamera)
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

			ProcessMesh(meshComp->mesh, meshComp->isVisible, meshComp->drawWireMesh, mainCameraPosition, transformComp->transformMatrix, viewProjMatrix);

#ifdef DEBUG
			if (boundingVolume && boundingVolume->drawBoundingVolume)
				DrawBoundingVolume(boundingVolume->mesh, boundingVolume->volumeRadius, mainCameraPosition, transformComp->transformMatrix, viewProjMatrix);
#endif
		}

		Renderer::GetInstance().FreeResourceOverflow();
	}

	void RenderSystem::ProcessMesh(Mesh& mesh, bool isVisible, bool drawWireFrame, const Math::Vector3 cameraPosition, const Math::Matrix4x4& transformMatrix, const Math::Matrix4x4& viewProjMatrix)
	{
		ReserveResources(mesh.totalPrimitives, drawWireFrame);

		for (int i = 0; i < mesh.totalPrimitives; i++)
		{
			Math::Vector4 vertex[4] = { {} };
			Primitives& primitive = mesh.meshPrimitives[i];
			PrimitiveType type = primitive.primitiveType;
			int numVerticies = static_cast<int>(type);

			TransformVertex(primitive, transformMatrix, vertex, numVerticies);
			bool cullPrimitive = CullBackFace(cameraPosition, vertex);

			if (cullPrimitive && !drawWireFrame)
			{
				continue;
			}

			ProcessVertex(viewProjMatrix, vertex, numVerticies);

			if (!IsPrimitiveWithinFrustrum(numVerticies, vertex))
			{
				continue;
			}

			if (drawWireFrame)
			{
				AddWireMeshToRenderer(numVerticies, vertex);
			}

			if (!cullPrimitive && isVisible)
			{
				AddPrimitiveToRenderer(primitive, numVerticies, vertex);
			}
		}
	}


	bool RenderSystem::CullBackFace(const Math::Vector3& cameraLocation, const Math::Vector4 primitiveVerts[])
	{
		Math::Vector3 normal = Math::Vector3(Math::Vector4::Cross(primitiveVerts[1] - primitiveVerts[0], primitiveVerts[2] - primitiveVerts[0]));
		const Math::Vector3 cam = cameraLocation - Math::Vector3(primitiveVerts[0]);
		return (Math::Vector3::Dot(normal, cam)) <= 0;
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

	void RenderSystem::DrawBoundingVolume(Mesh& mesh, const float radius, const Math::Vector3& cameraPosition, const Math::Matrix4x4& transformMatrix, const Math::Matrix4x4& viewProjMatrix)
	{
		Math::Matrix4x4 boundingMatrix{};

		for (int i = 0; i < 3; i++)
		{
			boundingMatrix[i][i] = radius;
			boundingMatrix[3][i] = transformMatrix[3][i];
		}

		ProcessMesh(mesh, false, true, cameraPosition, boundingMatrix, viewProjMatrix);
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
			vertex[j] = Math::Vector4(primitive.verticies[j], 1.0f);
			vertex[j] = transformMatrix * vertex[j];
		}
	}

	void RenderSystem::ProcessVertex(const Math::Matrix4x4& viewProjMatrix, Math::Vector4 vertex[], int numVerticies)
	{
		for (int j = 0; j < numVerticies; j++)
		{
			vertex[j] = viewProjMatrix * vertex[j];
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
		return depth;
	}
}