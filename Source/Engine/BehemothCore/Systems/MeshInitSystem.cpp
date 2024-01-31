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
		for (const auto& [entity, meshInitComp] : registry.Get<MeshInitializeComponent>())
		{
			MeshComponent* meshComponent = registry.GetComponent<MeshComponent>(entity);
			if (meshComponent)
			{
				InitMesh(meshComponent->mesh);

				// Generates a bounding sphere to encompass the mesh for frustum culling
				if (meshInitComp->initBoundingVolume)
				{
					InitSphereBoundingVolume(registry, meshComponent, entity);
				}
			}

#ifdef DEBUG
			// Only need to initialize wire frame meshes when debugging
			WireframeComponent* wireframeMesh = registry.GetComponent<WireframeComponent>(entity);
			if (wireframeMesh)
			{
				InitMesh(wireframeMesh->mesh);
			}
#endif
			// Remove this component once initialization has completed
			registry.RemoveComponent<MeshInitializeComponent>(entity);
		}
	}

	void MeshInitSystem::InitMesh(Mesh& mesh)
	{
		const std::vector<VertexData>& vertexData = ResourceManager::GetInstance().GetMeshVertices(mesh.meshData.modelFileName);
		const MeshData& meshData = ResourceManager::GetInstance().GetMeshData(mesh.meshData.modelFileName);
		mesh.GenerateMesh(meshData, vertexData);
	}

	void MeshInitSystem::InitSphereBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& handle)
	{
		registry.AddComponent<BoundingVolumeComponent>(handle, true);
		BoundingVolumeComponent* boundingVolume = registry.GetComponent<BoundingVolumeComponent>(handle);
		if (boundingVolume)
		{

#ifdef DEBUG
			InitMesh(boundingVolume->mesh);
#endif
			SphereCollider collider = ResourceManager::GetInstance().GetMeshSphereBounds(meshComp->modelFileName);
			boundingVolume->radius = collider.radius;
			boundingVolume->localPosition = collider.center;
		}
	}
}