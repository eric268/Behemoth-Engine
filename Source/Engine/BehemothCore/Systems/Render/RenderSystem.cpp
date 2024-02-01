#include "pch.h"
#include "RenderSystem.h"
#include "ECS/Registry.h"
#include "Geometry/Primitives.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"

namespace Behemoth
{
	bool RenderSystem::IsPrimitiveWithinFrustrum(const int numVertices, BMath::Vector4 primitiveVerts[])
	{
		// Only want to cull primitives that are entirely outside of view frustum since OpenGL render pipeline will clip
		// the quads if portions are outside the frustum. Me doing that, potentially creating temporary primitives will
		// just increase draw calls with is counter intuitive 
		int numVerticesOutsideFrustrum = 0;
		for (int i = 0; i < numVertices; i++)
		{
			if (primitiveVerts[i].x < -primitiveVerts[i].w ||
				primitiveVerts[i].x >  primitiveVerts[i].w ||
				primitiveVerts[i].y < -primitiveVerts[i].w ||
				primitiveVerts[i].y >  primitiveVerts[i].w ||
				primitiveVerts[i].z >  primitiveVerts[i].w ||
				primitiveVerts[i].z < -primitiveVerts[i].w)
				numVerticesOutsideFrustrum++;
		}

		return numVerticesOutsideFrustrum != numVertices;
	}

	bool RenderSystem::IsBoundingVolumeInFrustrum(
		const CameraComponent* cameraComp, 
		const TransformComponent* entityTransformComp, 
		const BoundingVolumeComponent* boundingComp)
	{
		float maxScale = std::max(entityTransformComp->worldScale[0],
			       std::max(entityTransformComp->worldScale[1], entityTransformComp->worldScale[2]));

		for (const auto& p : cameraComp->worldSpaceFrustum)
		{
			float distance = BMath::Vector3::Dot(entityTransformComp->worldPosition + boundingComp->localPosition, p.normal) + p.d;
			float radius = boundingComp->radius * maxScale;
			if (distance < -radius)
			{
				 return false;
			}
		}
		return true;
	}

	void RenderSystem::TransformVertex(const Primitive& primitive, const BMath::Matrix4x4& transformMatrix, BMath::Vector4 vertex[], const int numVertices)
	{
		for (int j = 0; j < numVertices; j++)
		{
			vertex[j] = transformMatrix * primitive.vertices[j];
		}
	}

	float RenderSystem::ProcessVertex(const BMath::Matrix4x4& viewProjMatrix, BMath::Vector4 vertex[], int numVertices)
	{
		float depth = 0.0f;

		for (int j = 0; j < numVertices; j++)
		{
			vertex[j] = viewProjMatrix * vertex[j];

			if (vertex[j].w == 0.0f)
			{
				vertex[j].w = EPSILON;
			}
			depth += vertex[j].w;

			// Invert w so only perform 1 divide instead of 4
			float w = 1.0f / vertex[j].w;
			vertex[j] *= w;
		}

		return depth /= numVertices;
	}
}