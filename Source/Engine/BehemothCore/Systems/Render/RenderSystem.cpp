#include "pch.h"
#include "RenderSystem.h"
#include "Geometry/Primitives.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"

namespace Behemoth
{
	bool RenderSystem::IsPrimitiveWithinFrustrum(const int numVerticies, BMath::Vector4 primitiveVerts[])
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
				primitiveVerts[i].z < 0.0f)
				numVerticiesOutsideFrustrum++;
		}

		return numVerticiesOutsideFrustrum != numVerticies;
	}

	bool RenderSystem::IsBoundingVolumeInFrustrum(const CameraComponent* cameraComponent, const TransformComponent* entityTransform, const BoundingVolumeComponent* boundingComp)
	{
		float maxScale = std::max(entityTransform->worldScale[0], std::max(entityTransform->worldScale[1], entityTransform->worldScale[2]));
		for (const auto& p : cameraComponent->worldSpaceFrustum)
		{
			float distance = BMath::Vector3::Dot(entityTransform->worldPosition + boundingComp->localPosition, p.normal) + p.d;
			float radius = boundingComp->radius * maxScale;
			if (distance < -radius )
			{
				 return false;
			}
		}
		return true;
	}

	void RenderSystem::TransformVertex(const Primitive& primitive, const BMath::BMatrix4x4& transformMatrix, BMath::Vector4 vertex[], const int numVerticies)
	{
		for (int j = 0; j < numVerticies; j++)
		{
			vertex[j] = transformMatrix * primitive.verticies[j];
		}
	}

	float RenderSystem::ProcessVertex(const BMath::BMatrix4x4& viewProjMatrix, BMath::Vector4 vertex[], int numVerticies)
	{
		float depth = 0.0f;

		for (int j = 0; j < numVerticies; j++)
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

		return depth /= numVerticies;
	}
}