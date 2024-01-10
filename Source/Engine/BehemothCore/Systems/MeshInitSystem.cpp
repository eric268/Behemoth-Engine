#include "pch.h"
#include "MeshInitSystem.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Components/PhysicsComponents.h"
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

		for (const auto& [entity, meshInitComp] : components)
		{
			MeshComponent* meshComponent = registry.GetComponent<MeshComponent>(entity);
			if (meshComponent)
			{
				InitMesh(meshComponent->mesh);

				// Make function for this
				if (meshInitComp->initBoundingVolume)
				{
					registry.AddComponent<BoundingVolumeComponent>(entity, true);
					BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(entity);
					if (boundingVolume)
					{
						InitMesh(boundingVolume->mesh);
						SphereCollider collider = ResourceManager::GetInstance().GetMeshSphereBounds(meshComponent->modelFileName);
						boundingVolume->radius = collider.radius;
						boundingVolume->localPosition = collider.position;
					}
				}

				// Make function for this
				if (meshInitComp->initBroadCollider)
				{
					registry.AddComponent<BroadColliderComponent>(entity);
					BroadColliderComponent* broadCollider = registry.GetComponent<BroadColliderComponent>(entity);
					if (broadCollider)
					{
						broadCollider->collider = ResourceManager::GetInstance().GetMeshAABBBounds(meshComponent->modelFileName);
					}
				}
			}

#ifdef DEBUG

			WireframeComponent* wireframeMesh = registry.GetComponent<WireframeComponent>(entity);
			if (wireframeMesh)
			{
				InitMesh(wireframeMesh->mesh);
			}
#endif
		}

		auto componentsToRemove = registry.GetComponent<MeshInitalizeComponent>();
		// Get all meshInitalizeComponents to be removed. Don't worry about also checking for if they have a MeshComponent because possible to add a MeshInitalizeComponent
		// without adding a MeshComponet. In this case we would just remove the MeshInitalizeComponent
		
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