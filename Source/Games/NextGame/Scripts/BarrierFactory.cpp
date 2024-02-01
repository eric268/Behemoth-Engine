#include "pch.h"
#include "BarrierFactory.h"
#include "Factories/GameObjectFactory.h"

ECS::EntityHandle BarrierFactory::CreateObstacle(
	ECS::Registry& registry,
	const BMath::Vector3& location,
	const BMath::Vector3& scale,
	const BMath::Quaternion& q,
	bool isStatic)
{
	ECS::EntityHandle obstacleHandle = Behemoth::GameObjectFactory::CreateGameObject(
		registry, 
		"cube10.obj", 
		"brick.png", 
		"Obstacle",
		BMath::Vector2(4, 4));

	registry.AddComponent<Behemoth::ScalingComponent>(obstacleHandle, scale);
	registry.AddComponent<Behemoth::MoveComponent>(obstacleHandle, location);
	registry.AddComponent<Behemoth::RotationComponent>(obstacleHandle, q, true);

	if (BMath::Quaternion::Equals(q, BMath::Quaternion::Identity()))
	{
		Behemoth::AABBColliderComponent* obstacleColliderComp = registry.AddComponent<Behemoth::AABBColliderComponent>(obstacleHandle);
		obstacleColliderComp->physicsMaterial = Behemoth::PhysicsMaterial(0.75, 1.0f);
	}
	else
	{
		Behemoth::OBBColliderComponent* obstacleColliderComp = registry.AddComponent<Behemoth::OBBColliderComponent>(obstacleHandle);
		obstacleColliderComp->physicsMaterial = Behemoth::PhysicsMaterial(0.75, 1.0f);
	}
	
	if (isStatic)
	{
		registry.AddComponent<Behemoth::StaticComponent>(obstacleHandle);
	}
	else
	{
		registry.AddComponent<Behemoth::RigidBodyComponent>(obstacleHandle);
	}

    return obstacleHandle;
}
