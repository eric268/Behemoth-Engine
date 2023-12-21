#pragma once

#include "ECS/System.h"

#include <tuple>
#include <vector>

namespace Behemoth
{
	class CameraComponent;
	class TransformComponent;
	class Entity;

	class RenderSystem
	{
	public:
		RenderSystem() = default;
		void Run(ECS::Registry& registry);

	private:


		void ProcessMesh(Mesh& mesh, bool isVisible, bool drawWireMesh, const Math::Vector3 cameraPosition, const Math::Matrix4x4& transform, const Math::Matrix4x4& viewProjMatrix, bool dirty);
		void DrawBoundingVolume(Mesh& mesh, const float radius, const Math::Vector3& cameraPosition, const Math::Matrix4x4& transform, const Math::Matrix4x4& viewProjMatrix, bool dirty);
		
		bool CullBackFace(const Math::Vector3& cameraLocation, const Math::Vector4 primitiveVerts[]);
		bool IsPrimitiveWithinFrustrum(const int numVerticies, Math::Vector4 primitiveVerts[]);
		bool IsBoundingVolumeInFrustrum(const CameraComponent* cameraComponent, const TransformComponent* transformComp, const float boundingRadius);
		void AddWireMeshToRenderer(const int numVerticies, const Math::Vector4 verticies[]);
		void AddPrimitiveToRenderer(Primitives& primitive, const int numVerticies, const Math::Vector4 verticies[]);
		void ReserveResources(int numPrimitives, bool drawWireFrame);
		void TransformVertex(const Primitives& primitive, const Math::Matrix4x4& transformMatix, Math::Vector4 vertex[], const int numVerticies);
		void ProcessVertex(const Math::Matrix4x4& viewProjMatrix, Math::Vector4 verticies[], int numVerticies);
		float GetPrimitiveDepth(const int numVerticies, const Math::Vector4 verticies[]);

		int counter = 0;

		std::vector<VertexData> cachedVerticies;
		std::string cachedMeshName;
	};
}