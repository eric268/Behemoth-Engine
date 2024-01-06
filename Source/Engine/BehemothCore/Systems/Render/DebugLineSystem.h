#pragma once
#include "Math/MathCore.h"
#include "ECS/Entity.h"

#include <vector>

namespace ECS
{
	class Registry;
};

namespace Behemoth
{
	class DebugLineComponent;
	class Line;

	class DebugLineSystem
	{
	public:
		using Point = Math::Vector3;

		DebugLineSystem() = default;
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void ReserveResources(int numLines);
		bool CullLineSegement(const Point& p1, const Point& p2, const Math::Plane* worldFrustmPlanes);
		void ProcessLine(const Point&, const Point& p2, const Math::Matrix4x4& viewProjMatrix, Math::Vector3 color);
		void AddLineToRenderer(const Line& line);
		void UpdateLineDisplayLifetime(const float deltaTime, std::vector<ECS::EntityHandle>& linesToDestroy, ECS::EntityHandle handle, DebugLineComponent* lineComponent);
		void DestroyExpiredLines(ECS::Registry& registry, std::vector<ECS::EntityHandle>& linesToDestroy);
	};
}

