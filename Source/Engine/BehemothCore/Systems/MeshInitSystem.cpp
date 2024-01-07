#include "pch.h"
#include "MeshInitSystem.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "ECS/Entity.h"
#include "Geometry/Mesh.h"
#include "Application/ResourceManager.h"
#include "Misc/Log.h"

namespace Behemoth
{
	void MeshInitSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		// Will need to get the sparse set container no matter what since the .Get<>() returns a tuple and iterating backwards is difficult
		// This way we can do a very quick and easy check to see if there are any meshes that need to be initialized
		// most likely this check will fail and exit early so want to do this before calling .Get on the registry

		auto components = registry.Get<MeshInitalizeComponent>();
		for (const auto& [entity, meshComp] : components)
		{
			MeshComponent* meshComponent = registry.GetComponent<MeshComponent>(entity);
			if (meshComponent)
			{
				InitMesh(meshComponent->mesh);
			}

#ifdef DEBUG
			BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(entity);
			if (boundingVolume)
			{
				InitMesh(boundingVolume->mesh);
			}

			WireframeComponent* wireframeMesh = registry.GetComponent<WireframeComponent>(entity);
			if (wireframeMesh)
			{
				InitMesh(wireframeMesh->mesh);
			}
#endif

		}

		auto componentsToRemove = registry.GetComponent<MeshInitalizeComponent>();

		// Iterate over the component backwards because we always swap a component to the back of the container before removing it
		// iterating over backwards allows us to update the container size and not cause errors in the range of the loop
		for (int i = componentsToRemove->dense.size() - 1; i >= 0; i--)
		{
			componentsToRemove->RemoveComponent(componentsToRemove->dense[i]);
		}

	}

	void MeshInitSystem::InitMesh(Mesh& mesh)
	{
		const std::vector<VertexData>& vertexData = ResourceManager::GetInstance().GetMeshVerticies(mesh.meshData.modelFileName);
		const MeshData& meshData = ResourceManager::GetInstance().GetMeshData(mesh.meshData.modelFileName);
		mesh.GenerateMesh(meshData, vertexData );
	}

}