#pragma once
#include "Math/MathCore.h"
#include "ECS/Entity.h"

#include "Systems/Render/RenderSystem.h"
#include <vector>

namespace ECS
{
	class Registry;
};

namespace Behemoth
{
	struct DebugLineComponent;
	struct Line;
	class Plane;

	class DebugLineSystem : public RenderSystem
	{
	public:
		using Point = BMath::Vector3;

		DebugLineSystem() = default;
		void Run(const float deltaTime, ECS::Registry& registry);

	private:
		void ReserveResources(int numLines);
		bool CullLineSegement(Point& p1, Point& p2, const Plane* worldFrustmPlanes);
		void ProcessLine(const Point&, const Point& p2, const BMath::BMatrix4x4& viewProjMatrix, BMath::Vector3 color);
		void AddLineToRenderer(Line& line);
		void DestroyExpiredLines(ECS::Registry& registry, std::vector<ECS::EntityHandle>& linesToDestroy);

		void UpdateLineDisplayLifetime(
			const float deltaTime,
			std::vector<ECS::EntityHandle>& linesToDestroy,
			ECS::EntityHandle handle, 
			DebugLineComponent* lineComponent);
	};
}

