#include "pch.h"
#include "MeshInitSystem.h"
#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "Components/Components.h"
#include "Components/RenderComponents.h"
#include "Components/PhysicsComponents.h"
#include "Geometry/Mesh.h"
#include "Core/ResourceManager.h"
#include "Core/Log.h"
#include "Misc/TransformHelper.h"



namespace Behemoth
{
	void MeshInitSystem::Run(const float deltaTime, ECS::Registry& registry)
	{
		for (const auto& [entity, meshInitComp] : registry.Get<MeshInitalizeComponent>())
		{
			MeshComponent* meshComponent = registry.GetComponent<MeshComponent>(entity);
			if (meshComponent)
			{
				InitMesh(meshComponent->mesh);

				// Automatically generates an AABB collider for bounding volume hierarchy collision detection (broad collision detection)
// 				if (meshInitComp->initBroadCollider)
// 				{
// 					InitAABBBoundingVolume(registry, meshComponent, entity);
// 				}
				// Automatically generates a bounding sphere to encompass the mesh for frustum culling
				if (meshInitComp->initBoundingVolume)
				{
					InitSphereBoundingVolume(registry, meshComponent, entity);
				}
			}
			// Only need to initialize wire frame meshes when debugging
#ifdef DEBUG
			WireframeComponent* wireframeMesh = registry.GetComponent<WireframeComponent>(entity);
			if (wireframeMesh)
			{
				InitMesh(wireframeMesh->mesh);
			}
#endif
			// Remove this component once initialization has completed
			registry.RemoveComponent<MeshInitalizeComponent>(entity);
		}
	}

	void MeshInitSystem::InitMesh(Mesh& mesh)
	{
		const std::vector<VertexData>& vertexData = ResourceManager::GetInstance().GetMeshVertices(mesh.meshData.modelFileName);
		const MeshData& meshData = ResourceManager::GetInstance().GetMeshData(mesh.meshData.modelFileName);
		mesh.GenerateMesh(meshData, vertexData);
	}

// 	void MeshInitSystem::GenerateAABBBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle)
// 	{
// 		registry.AddComponent<BVHColliderComponent>(handle);
// 		BVHColliderComponent* broadCollider = registry.GetComponent<BVHColliderComponent>(handle);
// 		if (broadCollider)
// 		{
// 			BMath::Vector3 scale = BMath::Vector3::One();
// 
// 			if (TransformComponent* transformComp = registry.GetComponent<TransformComponent>(handle))
// 			{
// 				scale = transformComp->worldScale;
// 			}
// 			else
// 			{
// 				LOGMESSAGE(Error, "Error getting transform from entity: " + registry.GetName(handle));
// 			}
// 			BMath::Vector3 temp = ResourceManager::GetInstance().GetMeshAABBBounds(meshComp->modelFileName).extents;
// 			
// 			broadCollider->extents = temp;
// 			broadCollider->collider.extents = temp;
// 		}
// 	}

	void MeshInitSystem::InitSphereBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle)
	{
		registry.AddComponent<BoundingVolumeComponent>(handle, true);
		BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(handle);
		if (boundingVolume)
		{
			// InitMesh(boundingVolume->mesh, std::ref(Path));
			SphereCollider collider = ResourceManager::GetInstance().GetMeshSphereBounds(meshComp->modelFileName);
			boundingVolume->radius = collider.radius;
			boundingVolume->localPosition = collider.center;
		}
	}

// 	void MeshInitSystem::InitAABBBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle)
// 	{
// 		BVHColliderComponent* broadCollider = registry.AddComponent<BVHColliderComponent>(handle);
// 		if (broadCollider)
// 		{
// 			BMath::Vector3 scale = BMath::Vector3::One();
// 			broadCollider->collider = ResourceManager::GetInstance().GetMeshAABBBounds(meshComp->modelFileName);
// 			// registry.AddComponent<Behemoth::WireframeComponent>(handle, "cube.obj", broadCollider->extents, false);
// 		}
// 	}
// 
// 	void MeshInitSystem::GetRotatedAABB(const AABBCollider& a, const BMath::Matrix3x3& rotation,  AABBCollider& result)
// 	{
// 		for (int i = 0; i < 3; i++)
// 		{
// 			result.extents[i] = 0.0f;
// 			for (int j = 0; j < 3; j++)
// 			{
// 				result.extents[i] += std::abs(rotation.data[i][j]) * a.extents[i];
// 			}
// 		}
// 	}
}