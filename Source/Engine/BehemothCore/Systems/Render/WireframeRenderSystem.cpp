#include "pch.h"
#include "WireframeRenderSystem.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Misc/CameraHelper.h"
#include "Renderer/Renderer.h"
#include "Application/ResourceManager.h"
#include "Geometry/Line.h"
#include "Misc/TransformHelper.h"

namespace Behemoth
{
	void WireframeRenderSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<WireframeComponent, TransformComponent>();

		CameraComponent* mainCamera = CameraHelper::GetMainCamera(registry);
		BMath::Vector3 mainCameraPosition = CameraHelper::GetMainCameraPostition(registry);

		std::sort(components.begin(), components.end(),
			[&](std::tuple<ECS::Entity, WireframeComponent*, TransformComponent*> tuple1, std::tuple<ECS::Entity, WireframeComponent*, TransformComponent*> tuple2)
			{
				return std::get<1>(tuple1)->mesh.meshPrimitives.size() < std::get<1>(tuple2)->mesh.meshPrimitives.size();
			});

		// ** Order of multiplication matters here **
		BMath::Matrix4x4 viewProjMatrix = mainCamera->projMatrix * mainCamera->viewMatrix;

		for (const auto& [entity, wireframeComp, transformComp] : components)
		{
			if (!wireframeComp->isVisible)
				continue;

			BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(entity);
			if (boundingVolume && !IsBoundingVolumeInFrustrum(mainCamera, transformComp, boundingVolume))
			{
				continue;
			}
			const BMath::Matrix4x4 wireframeTranform = GetWireframeTransform(transformComp->worldTransform, transformComp->worldScale, wireframeComp->scale, wireframeComp->allowRotation);
			ProcessWireframe(wireframeComp->mesh, wireframeTranform, viewProjMatrix, true, wireframeComp->wireframeColor);
		}
	}

	void WireframeRenderSystem::ProcessWireframe(Mesh& mesh, const BMath::Matrix4x4f& transformMatrix, const BMath::Matrix4x4f& viewProjMatrix, bool isDirty, BMath::Vector3 color)
	{
		const MeshData& meshData = mesh.meshData;

		ReserveResources(meshData.totalPrimitives);

// 		if (isDirty && cachedMeshName != meshData.modelFileName)
// 		{
// 			cachedMeshName = meshData.modelFileName;
// 			cachedVerticies = ResourceManager::GetInstance().GetMeshVerticies(meshData.modelFileName);
// 		}
		std::vector<VertexData> cachedVerticies = ResourceManager::GetInstance().GetMeshVerticies(meshData.modelFileName);

		int numVerticies = 3;
		for (int i = 0, vertexIndex = 0; i < meshData.totalPrimitives; i++)
		{
			if (vertexIndex >= meshData.triangleVertexCount)
				numVerticies = 4;

			Primitive& primitive = mesh.meshPrimitives[i];

			// Only need to update the verticies if matrix dirty
			if (isDirty)
			{
				for (int j = 0; j < numVerticies; j++)
				{
					primitive.verticies[j] = transformMatrix * BMath::Vector4(cachedVerticies[vertexIndex].vertex, 1.0f);
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
			
			AddWireMeshToRenderer(numVerticies, renderVerts, color);
		}
	}

	void WireframeRenderSystem::AddWireMeshToRenderer(const int numVerticies, const BMath::Vector4 verticies[], BMath::Vector3 color)
	{
		for (int i = 0, j = i + 1; i < numVerticies; i++, j++)
		{
			j = (j >= numVerticies) ? 0 : j;
			Line line = Line(BMath::Vector4(verticies[i].x, verticies[i].y, verticies[j].x, verticies[j].y), color);
			Renderer::GetInstance().AddLine(std::move(line));
		}
	}

	void WireframeRenderSystem::ReserveResources(int numPrimitives)
	{
		Renderer::GetInstance().ReserveLines(numPrimitives * 4);
	}

	BMath::Matrix4x4f WireframeRenderSystem::GetWireframeTransform(const BMath::Matrix4x4f& ownerTransform, const BMath::Vector3& ownerWorldScale, const BMath::Vector3& wireframeScale, const bool allowRotation)
	{
		BMath::Matrix4x4 scaledMatrix = BMath::Matrix4x4f::Identity();
		for (int i = 0; i < 3; i++)
		{
			scaledMatrix.data[i][i] = wireframeScale[i];
		}
		return (allowRotation) ? ownerTransform * scaledMatrix : TransformHelper::GetTransformNoRotation(ownerTransform, ownerWorldScale * wireframeScale);
	}
}