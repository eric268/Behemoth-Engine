#include "pch.h"
#include "MeshInitSystem.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Components/PhysicsComponents.h"
#include "ECS/Registry.h"
#include "Geometry/Mesh.h"
#include "Application/ResourceManager.h"
#include "Misc/Log.h"
#include "Misc/TransformHelper.h"

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


				if (meshInitComp->initBroadCollider)
				{
					CalculateRotatedAABB(registry, meshComponent, entity);
				}

				if (meshInitComp->initBoundingVolume)
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
			BMath::Vector3 temp = ResourceManager::GetInstance().GetMeshAABBBounds(meshComp->modelFileName).worldExtents;
			
			// temp *= 2.0f * std::sqrt(3) / 2.0f; This is to ensure that the cube stays within the aabb
			
			broadCollider->extents = temp;
			broadCollider->collider.worldExtents = temp;
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

	void MeshInitSystem::CalculateRotatedAABB(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle)
	{
		registry.AddComponent<BroadColliderComponent>(handle);
		BroadColliderComponent* broadCollider = registry.GetComponent<BroadColliderComponent>(handle);
		if (broadCollider)
		{
			BMath::Vector3 scale = BMath::Vector3::One();
			AABBCollider baseCollider = ResourceManager::GetInstance().GetMeshAABBBounds(meshComp->modelFileName);
			AABBCollider rotatedCollider{};

			if (TransformComponent* transformComp = registry.GetComponent<TransformComponent>(handle))
			{
				scale = transformComp->worldScale;
				BMath::Matrix3x3 rot = TransformHelper::ExtractRotationMatrix(transformComp->worldTransform);

				GetRotatedAABB(baseCollider, rot,  rotatedCollider);
			}
			else
			{
				LOGMESSAGE(Error, "Error getting transform from entity: " + registry.GetName(handle));
			}


			// temp *= 2.0f * std::sqrt(3) / 2.0f; This is to ensure that the cube stays within the aabb

			broadCollider->extents = rotatedCollider.worldExtents;
			broadCollider->collider.worldExtents = broadCollider->extents;
		}
	}

	void MeshInitSystem::GetRotatedAABB(const AABBCollider& a, const BMath::Matrix3x3& rotation,  AABBCollider& result)
	{
		for (int i = 0; i < 3; i++)
		{
			result.worldExtents[i] = 0.0f;
			for (int j = 0; j < 3; j++)
			{
				result.worldExtents[i] += std::abs(rotation.data[i][j]) * a.worldExtents[i];
			}
		}
	}
}