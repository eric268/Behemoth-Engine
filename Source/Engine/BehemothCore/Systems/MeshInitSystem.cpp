#include "pch.h"
#include "MeshInitSystem.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Components/PhysicsComponents.h"
#include "ECS/Registry.h"
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
					GenerateAABBBoundingVolume(registry, meshComponent, entity);
				}

				// Make function for this
				if (meshInitComp->initBroadCollider)
				{
					GenerateSphereBoundingVolume(registry, meshComponent, entity);
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

	void MeshInitSystem::GenerateAABBBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle)
	{
		registry.AddComponent<BroadColliderComponent>(handle);
		BroadColliderComponent* broadCollider = registry.GetComponent<BroadColliderComponent>(handle);
		if (broadCollider)
		{
			BMath::Vector3 scale = BMath::Vector3::One();

			if (TransformComponent* transformComp = registry.GetComponent<TransformComponent>(handle))
			{
				scale = transformComp->worldScale;
			}
			else
			{
				LOGMESSAGE(Error, "Error getting transform from entity: " + registry.GetName(handle));
			}

			broadCollider->extents = ResourceManager::GetInstance().GetMeshAABBBounds(meshComp->modelFileName).worldExtents;
			broadCollider->collider.worldExtents = broadCollider->extents;
		}
	}
	void MeshInitSystem::GenerateSphereBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle)
	{
		registry.AddComponent<BoundingVolumeComponent>(handle, false);
		BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(handle);
		if (boundingVolume)
		{
			InitMesh(boundingVolume->mesh);
			SphereCollider collider = ResourceManager::GetInstance().GetMeshSphereBounds(meshComp->modelFileName);
			boundingVolume->radius = collider.radius;
			boundingVolume->localPosition = collider.position;
		}
	}

}