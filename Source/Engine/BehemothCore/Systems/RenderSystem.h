#pragma once

#include "ECS/System.h"

namespace Behemoth
{
	class RenderSystem : public ECS::System
	{
	public:
		RenderSystem() = default;

		virtual void Run(ECS::Registry& registry) override;
	private:

		bool CullBackFace(const Math::Vector3& cameraLocation, const Math::Vector4 primitiveVerts[]);
	};
}