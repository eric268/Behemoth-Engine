#pragma once

#include "RenderSystem.h"
#include <tuple>
#include <vector>

namespace Behemoth
{
	class CameraComponent;
	class TransformComponent;
	class Entity;

	class MeshRenderSystem : public RenderSystem
	{
	public:
		MeshRenderSystem() = default;
		void Run(const float deltaTime, ECS::Registry& registry) override;

	protected:
		void ReserveResources(int numPrimitives) override;
		void ProcessMesh(Mesh& mesh, const BMath::Vector3 cameraPosition, const BMath::Matrix4x4& transform, const BMath::Matrix4x4& viewProjMatrix, bool dirty);
		bool CullBackFace(const BMath::Vector3& cameraLocation, const BMath::Vector4 primitiveVerts[]);
		void AddPrimitiveToRenderer(Primitives& primitive, const int numVerticies, const BMath::Vector4 verticies[]);
		float GetPrimitiveDepth(const int numVerticies, const BMath::Vector4 verticies[]);
	};
}