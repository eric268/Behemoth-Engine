#include "pch.h"
#include "RenderSystem.h"
#include "Geometry/Primitives.h"
#include "Components/Components.h"

namespace Behemoth
{
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
				primitiveVerts[i].z > primitiveVerts[i].w ||
				primitiveVerts[i].z <= 0.0f)
				numVerticiesOutsideFrustrum++;
		}

		return numVerticiesOutsideFrustrum != numVerticies;
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
}