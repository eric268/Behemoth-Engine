#include "MeshInitSystem.h"
#include "Components/Components.h"
#include "Render/Mesh.h"
#include "Application/ResourceManager.h"
#include "Misc/Log.h"

namespace Behemoth
{
	void MeshInitSystem::Run(ECS::Registry& registry)
	{
		// Will need to get the sparse set container no matter what since the .Get<>() returns a tuple and iterating backwards is difficult
		// This way we can do a very quick and easy check to see if there are any meshes that need to be initialized
		// most likely this check will fail and exit early so want to do this before calling .Get on the registry


		auto eraseInitComponents = registry.GetComponent<MeshInitalizeComponent>();
		if (!eraseInitComponents->size())
			return;

		auto components = registry.Get<MeshComponent>();
		for (const auto& [meshComp] : components)
		{
			const MeshData* data = ResourceManager::GetInstance().GetMesh(meshComp->modelFileName);
			if (data)
			{
				meshComp->mesh.GenerateMesh(data->triangleData, data->quadData);
			}
			else
			{
				LOG_ERROR("Error loading mesh");
			}
		}


		// Iterate over the component backwards because we always swap a component to the back of the container before removing it
		// iterating over backwards allows us to update the container size and not cause errors in the range of the loop
		for (int i = eraseInitComponents->dense.size() - 1; i >= 0; i--)
		{
			eraseInitComponents->RemoveComponent(eraseInitComponents->dense[i]);
		}

	}

}