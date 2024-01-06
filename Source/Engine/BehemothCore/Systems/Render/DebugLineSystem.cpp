#include "pch.h"
#include "ECS/Registry.h"
#include "DebugLineSystem.h"
#include "Components/RenderComponents.h"
#include "Components/Components.h"
#include "Misc/CameraHelper.h"
#include "Renderer/Renderer.h"
#include "Renderer/Line.h"

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
		ReserveResources(components.size());

		CameraComponent* mainCamera = CameraHelper::GetMainCamera(registry);
		Math::Vector3 mainCameraPosition = CameraHelper::GetMainCameraPostition(registry);
		Math::Matrix4x4 viewProjMatrix = mainCamera->perspectiveMatrix * mainCamera->viewMatrix;

		std::vector<ECS::EntityHandle> expiredLines;

		for (const auto& [entity, lineComp] : components)
		{
			if (CullLineSegement(lineComp->startPoint, lineComp->endPoint, mainCamera->worldSpaceFrustum))
			{
				continue;
			}

			ProcessLine(lineComp->startPoint, lineComp->endPoint, viewProjMatrix, lineComp->color);
			UpdateLineDisplayLifetime(deltaTime, expiredLines, entity, lineComp);
		}

		DestroyExpiredLines(registry, expiredLines);
	}


	bool DebugLineSystem::CullLineSegement(const Point& p1, const Point& p2, const Math::Plane* worldFrustmPlanes)
	{
		for (int i = 0; i < 6; i++)
		{
			// Check if endpoints of line are inside frustum, if true do not cull frustum
			float dist1 = Math::Vector3::Dot(p1, worldFrustmPlanes->normal);
			if (dist1)
			{
				return false;
			}

			float dist2 = Math::Vector3::Dot(p2, worldFrustmPlanes->normal);
			if (dist2)
			{
				return false;
			}
		}
		// All end points are outside of the world frustum
		return true;
	}

	void DebugLineSystem::ReserveResources(int numLines)
	{
		Renderer::GetInstance().ReserveLines(numLines);
	}
	void DebugLineSystem::ProcessLine(const Point& p1, const Point& p2, const Math::Matrix4x4& viewProjMatrix, Math::Vector3 color)
	{
		Math::Vector4 transformedPoint1 = viewProjMatrix * Math::Vector4(p1, 1.0f);
		Math::Vector4 transformedPoint2 = viewProjMatrix * Math::Vector4(p2, 1.0f);

		transformedPoint1 = transformedPoint1 / transformedPoint1.w;
		transformedPoint2 = transformedPoint2 / transformedPoint2.w;

		Line line = Line(Math::Vector4(transformedPoint1.x, transformedPoint1.y, transformedPoint2.x, transformedPoint2.y), color);

		AddLineToRenderer(line);
	}
	void DebugLineSystem::AddLineToRenderer(const Line& line)
	{
		Renderer::GetInstance().AddLine(std::move(line));
	}

	void DebugLineSystem::UpdateLineDisplayLifetime(const float deltaTime, std::vector<ECS::EntityHandle>& linesToDestroy, ECS::EntityHandle handle, DebugLineComponent* lineComponent)
	{
		lineComponent->displayCounter += deltaTime;
		if (lineComponent->displayCounter >= lineComponent->lifetime)
		{
			linesToDestroy.push_back(handle);
		}
	}

	void DebugLineSystem::DestroyExpiredLines(ECS::Registry& registry, std::vector<ECS::EntityHandle>& linesToDestroy)
	{
		for (const ECS::EntityHandle handle : linesToDestroy)
		{
			registry.DestroyEntity(handle);
		}
	}
}
