#include "pch.h"
#include "ECS/Registry.h"
#include "DebugLineSystem.h"
#include "Components/RenderComponents.h"
#include "Components/Components.h"
#include "Misc/CameraHelper.h"
#include "Renderer/Renderer.h"
#include "Geometry/Line.h"
#include "Physics/Collision/BroadCollision.h"
#include "Geometry/Plane.h"

namespace Behemoth
{
	void DebugLineSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		auto components = registry.Get<DebugLineComponent>();

		int numDebugLines = components.size();
		if (!numDebugLines)
		{
			return;
		}

		// Maximum number of lines that we may need to reserve
		ReserveResources(numDebugLines);

		// This is inefficient to essentially call Get twice, but will only run in debug and is not production code
		CameraComponent* mainCamera = CameraHelper::GetMainCamera(registry);
		BMath::Vector3 mainCameraPosition = CameraHelper::GetMainCameraPosition(registry);
		BMath::Matrix4x4 viewProjMatrix = mainCamera->projMatrix * mainCamera->viewMatrix;

		std::vector<ECS::EntityHandle> expiredLines;

		for (const auto& [entityHandle, lineComp] : components)
		{
			UpdateLineDisplayLifetime(deltaTime, expiredLines, entityHandle, lineComp);

			Point p1 = lineComp->startPoint;
			Point p2 = lineComp->endPoint;

			if (CullLineSegment(p1, p2, mainCamera->worldSpaceFrustum))
			{
				continue;
			}

			ProcessLine(p1, p2, viewProjMatrix, lineComp->color);
		}
		DestroyExpiredLines(registry, expiredLines);
	}


	bool DebugLineSystem::CullLineSegment(Point& p1, Point& p2, const Plane* worldFrustumPlanes)
	{
		for (int i = 0; i < 6; i++)
		{
			const Plane& plane = worldFrustumPlanes[i];

			// Check if endpoints of line are inside frustum, if true do not cull frustum
			float dist1 = BMath::Vector3::Dot(p1, plane.normal) + plane.d;
			float dist2 = BMath::Vector3::Dot(p2, plane.normal) + plane.d;

			if (dist1 >= 0 && dist2 >= 0)
			{
				continue;
			}
			else if (dist1 < 0 && dist2 < 0)
			{
				 return true;
			}
			else
			{
				// One point is outside and one is inside, so adjust its position to be on the perimeter of the view frustum
				Point intersectionPoint{};
				float distance = 0.0f;
				if (Behemoth::BroadLinePlaneIntersection(p1, p2, worldFrustumPlanes[i], distance, intersectionPoint))
				{
					if (dist1 < 0)
					{
						p1 = intersectionPoint;
					}
					else
					{
						p2 = intersectionPoint;
					}
				}
			}
		}

		// All end points are outside of the view frustum
		return false;
	}

	void DebugLineSystem::ReserveResources(int numLines)
	{
		Renderer::GetInstance().ReserveLines(numLines);
	}

	void DebugLineSystem::ProcessLine(const Point& p1, const Point& p2, const BMath::Matrix4x4& viewProjMatrix, BMath::Vector3 color)
	{
		BMath::Vector4 renderVerts[2];

		renderVerts[0] = viewProjMatrix * BMath::Vector4(p1, 1.0f);
		renderVerts[1] = viewProjMatrix * BMath::Vector4(p2, 1.0f);

		renderVerts[0] = renderVerts[0] / renderVerts[0].w;
		renderVerts[1] = renderVerts[1] / renderVerts[1].w;

		Line line = Line(BMath::Vector4(renderVerts[0].x, renderVerts[0].y, renderVerts[1].x, renderVerts[1].y), color);

		AddLineToRenderer(line);
	}
	void DebugLineSystem::AddLineToRenderer(Line& line)
	{
		Renderer::GetInstance().AddLine(std::move(line));
	}

	void DebugLineSystem::UpdateLineDisplayLifetime(const float deltaTime, std::vector<ECS::EntityHandle>& linesToDestroy, ECS::EntityHandle entityHandle, DebugLineComponent* lineComponent)
	{
		lineComponent->displayCounter += deltaTime;
		if (lineComponent->displayCounter >= lineComponent->lifetime)
		{
			linesToDestroy.push_back(entityHandle);
		}
	}

	void DebugLineSystem::DestroyExpiredLines(ECS::Registry& registry, std::vector<ECS::EntityHandle>& linesToDestroy)
	{
		for (const ECS::EntityHandle entityHandle : linesToDestroy)
		{
			registry.DestroyEntity(entityHandle);
		}
	}
}
