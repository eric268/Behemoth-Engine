#include "pch.h"
#include "WireframeRenderSystem.h"
#include "ECS/Registry.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Misc/CameraHelper.h"
#include "Renderer/Renderer.h"
#include "Core/ResourceManager.h"
#include "Geometry/Line.h"
#include "Misc/TransformHelper.h"
#include "Core/Stopwatch.h"
#include "Geometry/Mesh.h"

namespace Behemoth
{
	void WireframeRenderSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<WireframeComponent, TransformComponent>();

		CameraComponent* mainCamera = CameraHelper::GetMainCamera(registry);
		BMath::Vector3 mainCameraPosition = CameraHelper::GetMainCameraPosition(registry);


		// ** Order of multiplication matters here **
		BMath::Matrix4x4 viewProjMatrix = mainCamera->projMatrix * mainCamera->viewMatrix;

		int totalPrimitives = 0;
		for (const auto& [entity, wireframeComp, transformComp] : components)
		{
			totalPrimitives += wireframeComp->mesh.meshPrimitives.size();
		}

		ReserveResources(totalPrimitives);

		for (const auto& [entityHandle, wireframeComp, transformComp] : components)
		{
			if (!wireframeComp->isVisible)
				continue;

			BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(entityHandle);
			if (boundingVolume && !IsBoundingVolumeInFrustrum(mainCamera, transformComp, boundingVolume))
			{
				continue;
			}
			const BMath::Matrix4x4 wireframeTransform = GetWireframeTransform(transformComp->worldTransform, transformComp->worldScale, wireframeComp->scale, wireframeComp->allowRotation);
			ProcessWireframe(wireframeComp->mesh, wireframeTransform, viewProjMatrix, true, wireframeComp->wireframeColor);
		}
	}

	void WireframeRenderSystem::ProcessWireframe(Mesh& mesh, const BMath::Matrix4x4& transformMatrix, const BMath::Matrix4x4& viewProjMatrix, bool isDirty, BMath::Vector3 color)
	{
		const MeshData& meshData = mesh.meshData;

		const std::vector<VertexData>& vertices = ResourceManager::GetInstance().GetMeshVertices(meshData.modelFileName);

		int numVertices = 3;
		for (int i = 0, vertexIndex = 0; i < meshData.totalPrimitives; i++)
		{
			if (vertexIndex >= meshData.triangleVertexCount)
			{
				numVertices = 4;
			}

			Primitive& primitive = mesh.meshPrimitives[i];

			// Only need to update the vertices if matrix dirty
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

			BMath::Vector4 renderVerts[4];
			memcpy(renderVerts, primitive.vertices, sizeof(BMath::Vector4) * 4);

			ProcessVertex(viewProjMatrix, renderVerts, numVertices);

			if (!IsPrimitiveWithinFrustrum(numVertices, renderVerts))
			{
				continue;
			}
			
			AddWireMeshToRenderer(numVertices, renderVerts, color);
		}
	}

	void WireframeRenderSystem::AddWireMeshToRenderer(const int numVertices, const BMath::Vector4 vertices[], BMath::Vector3 color)
	{
		for (int i = 0, j = i + 1; i < numVertices; i++, j++)
		{
			j = (j >= numVertices) ? 0 : j;
			Line line = Line(BMath::Vector4(vertices[i].x, vertices[i].y, vertices[j].x, vertices[j].y), color);
			Renderer::GetInstance().AddLine(std::move(line));
		}
	}

	void WireframeRenderSystem::ReserveResources(int numPrimitives)
	{
		Renderer::GetInstance().ReserveLines(numPrimitives * 4);
	}

	BMath::Matrix4x4 WireframeRenderSystem::GetWireframeTransform(const BMath::Matrix4x4& ownerTransform, const BMath::Vector3& ownerWorldScale, const BMath::Vector3& wireframeScale, const bool allowRotation)
	{
		BMath::Matrix4x4 scaledMatrix = BMath::Matrix4x4::Identity();
		for (int i = 0; i < 3; i++)
		{
			scaledMatrix.data[i][i] = wireframeScale[i];
		}
		return (allowRotation) ? ownerTransform * scaledMatrix : TransformHelper::GetTransformNoRotation(ownerTransform, ownerWorldScale * wireframeScale);
	}
}