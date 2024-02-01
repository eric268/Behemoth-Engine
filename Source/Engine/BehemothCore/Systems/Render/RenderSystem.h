#pragma once

#include "Math/Vector.h"
#include "Math/Matrix.h"

namespace ECS
{
	class Registry;
}

namespace Behemoth
{
	struct CameraComponent;
	struct TransformComponent;
	struct BoundingVolumeComponent;
	class Primitive;

	class RenderSystem
	{
	protected:
		virtual void ReserveResources(int numPrimitives) = 0;
		virtual void Run(const float deltaTime, ECS::Registry& registry) = 0;

		bool IsPrimitiveWithinFrustrum(const int numVertices, BMath::Vector4 primitiveVerts[]);
		bool IsBoundingVolumeInFrustrum(const CameraComponent* cameraComp, const TransformComponent* transformComp, const BoundingVolumeComponent* boundingComp);

		void TransformVertex(const Primitive& primitive, const BMath::Matrix4x4& transformMatix, BMath::Vector4 vertex[], const int numVertices);
		float ProcessVertex(const BMath::Matrix4x4& viewProjMatrix, BMath::Vector4 vertices[], int numVertices);
	};
}