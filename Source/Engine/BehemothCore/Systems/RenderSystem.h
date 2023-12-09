#pragma once

#include "ECS/System.h"

namespace Behemoth
{
	class CameraComponent;
	class FrustrumComponent;
	class TransformComponent;
	class Mesh;

	class RenderSystem : public ECS::System
	{
	public:
		RenderSystem() = default;

		virtual void Run(ECS::Registry& registry) override;
	private:
		
		void DrawMesh(const CameraComponent* cameraComponent, const Mesh& mesh, const TransformComponent* transformComponent, const FrustrumComponent* frustrumComponent);
		bool CullBackFace(const Math::Vector3& cameraLocation, const Math::Vector4 primitiveVerts[]);
		bool IsInFrustrum(const CameraComponent* cameraComponent, const FrustrumComponent* frustrumComp, const TransformComponent* transformComp, const float boundingRadius);
	};
}