#pragma once

#include "ECS/System.h"
#include "Geometry/Mesh.h"

namespace Behemoth
{
	class CameraComponent;
	class TransformComponent;

	class RenderSystem
	{
	public:
		RenderSystem() = default;

	protected:
		virtual void ReserveResources(int numPrimitives) = 0;
		virtual void Run(const float deltaTime, ECS::Registry& registry) = 0;

		bool IsPrimitiveWithinFrustrum(const int numVerticies, BMath::Vector4 primitiveVerts[]);
		bool IsBoundingVolumeInFrustrum(const CameraComponent* cameraComponent, const TransformComponent* transformComp, const float boundingRadius);

		void TransformVertex(const Primitives& primitive, const BMath::Matrix4x4& transformMatix, BMath::Vector4 vertex[], const int numVerticies);
		void ProcessVertex(const BMath::Matrix4x4& viewProjMatrix, BMath::Vector4 verticies[], int numVerticies);

		std::vector<VertexData> cachedVerticies;
		std::string cachedMeshName;
	};

}