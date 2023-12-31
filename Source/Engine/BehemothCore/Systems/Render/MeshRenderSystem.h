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
		void ProcessMesh(Mesh& mesh, const Math::Vector3 cameraPosition, const Math::Matrix4x4& transform, const Math::Matrix4x4& viewProjMatrix, bool dirty);
		bool CullBackFace(const Math::Vector3& cameraLocation, const Math::Vector4 primitiveVerts[]);
		void AddPrimitiveToRenderer(Primitives& primitive, const int numVerticies, const Math::Vector4 verticies[]);
		float GetPrimitiveDepth(const int numVerticies, const Math::Vector4 verticies[]);
	};
}