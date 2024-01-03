#include "pch.h"
#include "WireframeRenderSystem.h"
#include "Components/Components.h"
#include "Misc/CameraHelper.h"
#include "Renderer/Renderer.h"
#include "Application/ResourceManager.h"
#include "Renderer/Mesh.h"
#include "Renderer/Line.h"

namespace Behemoth
{
	void WireframeRenderSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<WireframeComponent, TransformComponent>();
		auto cameraComponents = registry.Get<CameraComponent, TransformComponent>();

		CameraComponent* mainCamera = CameraHelper::GetMainCamera(registry);
		Math::Vector3 mainCameraPosition = CameraHelper::GetMainCameraPostition(registry);

		std::sort(components.begin(), components.end(),
			[&](std::tuple<ECS::Entity, WireframeComponent*, TransformComponent*> tuple1, std::tuple<ECS::Entity, WireframeComponent*, TransformComponent*> tuple2)
			{
				return std::get<1>(tuple1)->mesh.meshPrimitives.size() < std::get<1>(tuple2)->mesh.meshPrimitives.size();
			});

		// ** Order of multiplication matters here **
		Math::Matrix4x4 viewProjMatrix = mainCamera->perspectiveMatrix * mainCamera->viewMatrix;

		for (const auto& [entity, wireframeComp, transformComp] : components)
		{
			if (!wireframeComp->isVisible)
				continue;

			BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(entity);
			if (boundingVolume && !IsBoundingVolumeInFrustrum(mainCamera, transformComp, boundingVolume->volumeRadius))
			{
				continue;
			}

			ProcessWireframe(wireframeComp->mesh, mainCameraPosition, transformComp->transformMatrix, viewProjMatrix, wireframeComp->scale, true, wireframeComp->wireframeColor);
		}
	}

	void WireframeRenderSystem::ProcessWireframe(Mesh& mesh, const Math::Vector3 cameraPosition, const Math::Matrix4x4& transformMatrix, const Math::Matrix4x4& viewProjMatrix, const Math::Vector3& scale, bool isDirty, Math::Vector3 color)
	{
		const MeshData& meshData = mesh.meshData;

		ReserveResources(meshData.totalPrimitives);

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
					primitive.verticies[j] = transformMatrix * Math::Vector4(cachedVerticies[vertexIndex].vertex * scale, 1.0f);
					vertexIndex++;
				}
			}

			Math::Vector4 renderVerts[4];
			memcpy(renderVerts, primitive.verticies, sizeof(Math::Vector4) * 4);

			ProcessVertex(viewProjMatrix, renderVerts, numVerticies);

			if (!IsPrimitiveWithinFrustrum(numVerticies, renderVerts))
			{
				continue;
			}
			
			AddWireMeshToRenderer(numVerticies, renderVerts, color);
		}
	}

	void WireframeRenderSystem::AddWireMeshToRenderer(const int numVerticies, const Math::Vector4 verticies[], Math::Vector3 color)
	{
		for (int i = 0, j = i + 1; i < numVerticies; i++, j++)
		{
			j = (j >= numVerticies) ? 0 : j;
			Line line = Line(Math::Vector4(verticies[i].x, verticies[i].y, verticies[j].x, verticies[j].y), color);
			Renderer::GetInstance().AddLine(std::move(line));
		}
	}

	void WireframeRenderSystem::ReserveResources(int numPrimitives)
	{
		Renderer::GetInstance().ReserveLines(numPrimitives * 4);
	}
}