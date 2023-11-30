

#include "System.h"
#include "Components.h"
#include "Application/ResourceManager.h"
#include "Render/Mesh.h"

#include <Windows.h>
#include "main.h"

using namespace BehemothEngine;
namespace ECS
{
	void RenderSystem::Run(Registry& registry)
	{
		auto components = registry.Get<MeshComponent>();

		for (const auto& [comp] : components)
		{
			std::size_t size = comp->mesh.meshPrimitives.size();
			for (int i = 0; i < size; i++)
			{
				comp->mesh.meshPrimitives[i].Draw();
			}
		}
	}


	void ModelLoadingSystem::Run(Registry& registry)
	{
		auto components = registry.Get<MeshComponent>();
		
		for (const auto& [ comp] : components)
		{
			const MeshData* data = ResourceManager::GetInstance().GetMesh(comp->filepath);
			comp->mesh.GeneratePrimitives(data->meshData);
		}
	}

	void CameraSystem::Run(Registry& registry)
	{
		auto components = registry.Get<CameraComponent>();

		for (auto& [comp] : components)
		{
			UpdateViewMatrix(*comp);
		}
	}

	void CameraSystem::UpdateViewMatrix(CameraComponent& component)
	{
		RECT rect;
		GetClientRect(MAIN_WINDOW_HANDLE, &rect);
		component.windowWidth = rect.right - rect.left;
		component.windowHeight = rect.bottom - rect.top;
	}
}