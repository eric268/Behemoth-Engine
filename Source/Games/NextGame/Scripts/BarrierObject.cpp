#include "pch.h"
#include "BarrierObject.h"
#include "Factories/GameObjectFactory.h"

ECS::EntityHandle BarrierObject::CreateBarrier(
	ECS::Registry& registry,
	const BMath::Vector3& location,
	const BMath::Vector3& scale,
	const BMath::Quaternion& q,
	bool isStatic)
{
	ECS::EntityHandle obstacleHandle = Behemoth::GameObjectFactory::CreateGameObject(registry, "cube.obj", "brick.png");
	registry.AddComponent<Behemoth::ScalingComponent>(obstacleHandle, scale);
	registry.AddComponent<Behemoth::MoveComponent>(obstacleHandle, location);
	registry.AddComponent<Behemoth::RotationComponent>(obstacleHandle, q, true);

	Behemoth::OBBColliderComponent* obstacleCollider = registry.AddComponent<Behemoth::OBBColliderComponent>(obstacleHandle);
	obstacleCollider->physicsMaterial = Behemoth::PhysicsMaterial(0.75, 1.0f);
	
	if (isStatic)
	{
		registry.AddComponent<Behemoth::StaticComponent>(obstacleHandle);
	}

    return obstacleHandle;
}
