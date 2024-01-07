#include "pch.h"
#include "WireframeRenderSystem.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Misc/CameraHelper.h"
#include "Renderer/Renderer.h"
#include "Application/ResourceManager.h"
#include "Geometry/Line.h"

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
		BMath::Matrix4x4 viewProjMatrix = mainCamera->perspectiveMatrix * mainCamera->viewMatrix;

		for (const auto& [entity, wireframeComp, transformComp] : components)
		{
			if (!wireframeComp->isVisible)
				continue;

			BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(entity);
			if (boundingVolume && !IsBoundingVolumeInFrustrum(mainCamera, transformComp, boundingVolume->volumeRadius))
			{
				continue;
			}

			ProcessWireframe(wireframeComp->mesh, transformComp->transformMatrix, viewProjMatrix, wireframeComp->scale, true, wireframeComp->wireframeColor);
		}
	}

	void WireframeRenderSystem::ProcessWireframe(Mesh& mesh, const BMath::Matrix4x4& transformMatrix, const BMath::Matrix4x4& viewProjMatrix, const BMath::Vector3& scale, bool isDirty, BMath::Vector3 color)
	{
		const MeshData& meshData = mesh.meshData;

		ReserveResources(meshData.totalPrimitives);

		if (isDirty && cachedMeshName != meshData.modelFileName)
		{
			cachedMeshName = meshData.modelFileName;
			cachedVerticies = ResourceManager::GetInstance().GetMeshVerticies(meshData.modelFileName);
		}

		const BMath::Matrix4x4 scaledTransformMatrix = GetScaledTransformMatrix(transformMatrix, scale);

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
					primitive.verticies[j] = scaledTransformMatrix * BMath::Vector4(cachedVerticies[vertexIndex].vertex, 1.0f);
					vertexIndex++;
				}
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

	BMath::Matrix4x4 WireframeRenderSystem::GetScaledTransformMatrix(const BMath::Matrix4x4& transformMatrix, const BMath::Vector3& scale)
	{
		BMath::Matrix4x4 scaledTransformMatrix = transformMatrix;

		for (int col = 0; col < 3; col++)
		{
			float length = sqrt(scaledTransformMatrix.data[col][0]  * scaledTransformMatrix.data[col][0]  +
									scaledTransformMatrix.data[col][1]  * scaledTransformMatrix.data[col][1]  +
									scaledTransformMatrix.data[col][2]  * scaledTransformMatrix.data[col][2]);

			if (length != 0)
			{
				for (int row = 0; row < 3; row++)
				{
					scaledTransformMatrix.data[col][row] /= length;
				}
			}
		}

		// Apply new scale
		for (int i = 0; i < 3; i++)
		{
			scaledTransformMatrix.data[i][i] = scale[i];
		}
		return scaledTransformMatrix;
	}
}