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
		for (const auto& [entityHandle, meshInitComp] : registry.Get<MeshInitializeComponent>())
		{
			if (MeshComponent* meshComp = registry.GetComponent<MeshComponent>(entityHandle))
			{
				InitMesh(meshComp->mesh);

				// Generates a bounding sphere to encompass the mesh for frustum culling
				if (meshInitComp->initBoundingVolume)
				{
					InitSphereBoundingVolume(registry, meshComp, entityHandle);
				}
			}

#ifdef DEBUG
			// Only need to initialize wire frame meshes when debugging
			WireframeComponent* wireframeComp = registry.GetComponent<WireframeComponent>(entityHandle);
			if (wireframeComp)
			{
				InitMesh(wireframeComp->mesh);
			}
#endif
			// Remove this component once initialization has completed
			registry.RemoveComponent<MeshInitializeComponent>(entityHandle);
		}
	}

	void MeshInitSystem::InitMesh(Mesh& mesh)
	{
		const std::vector<VertexData>& vertexData = ResourceManager::GetInstance().GetMeshVertices(mesh.meshData.modelFileName);
		const MeshData& meshData = ResourceManager::GetInstance().GetMeshData(mesh.meshData.modelFileName);
		mesh.GenerateMesh(meshData, vertexData);
	}

	void MeshInitSystem::InitSphereBoundingVolume(ECS::Registry& registry, MeshComponent* meshComp, const ECS::EntityHandle& entityHandle)
	{
		BoundingVolumeComponent* boundingVolumeComp = registry.AddComponent<BoundingVolumeComponent>(entityHandle, false);
		if (boundingVolumeComp)
		{
#ifdef DEBUG
			InitMesh(boundingVolumeComp->mesh);
#endif
			SphereCollider collider = ResourceManager::GetInstance().GetMeshSphereBounds(meshComp->modelFileName);
			boundingVolumeComp->radius = collider.radius;
			boundingVolumeComp->localPosition = collider.center;
		}
	}
}