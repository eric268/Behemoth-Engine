#include "pch.h"
#include "PlatformFactory.h"
#include "Factories/GameObjectFactory.h"
#include "Components/RenderComponents.h"
#include "PlatformPhysMats.h"
#include "Math/Quaternion.h"

ECS::EntityHandle PlatformFactory::CreatePlatform(
	ECS::Registry& registry,
	const Behemoth::PhysicsMaterial& physicsMaterial,
	const std::string& modelPath,
	const std::string& texturePath,
	const std::string& name,
	const BMath::Vector2& uvScale,
	bool isRotated)
{
	ECS::EntityHandle platform = Behemoth::GameObjectFactory::CreateGameObject(registry, modelPath, texturePath, name, uvScale);

	if (isRotated)
	{
		Behemoth::OBBColliderComponent* colliderComp = registry.AddComponent<Behemoth::OBBColliderComponent>(platform, BMath::Vector3(1.0f));
		if (colliderComp)
		{
			colliderComp->physicsMaterial = physicsMaterial;
		}
	}
	else
	{
		Behemoth::AABBColliderComponent* colliderComp = registry.AddComponent<Behemoth::AABBColliderComponent>(platform, BMath::Vector3(1.0f));
		if (colliderComp)
		{
			colliderComp->physicsMaterial = physicsMaterial;
		}
	}

	return platform;
}

ECS::EntityHandle PlatformFactory::CreateGrassPlatform(
	ECS::Registry& registry,
	const BMath::Vector3& position,
	const BMath::Vector3& scale,
	const BMath::Quaternion& q,
	bool isStatic)
{
	bool isRotated = BMath::Quaternion::Equals(q, BMath::Quaternion::Identity());

	ECS::EntityHandle platform = CreatePlatform(
		registry,
		PhysMat::platformPhysicsMats[PhysMat::Grass],
		"plane5.obj",
		"grass.png",
		"Grass Platform",
		BMath::Vector2(8,8),
		isRotated);

	registry.AddComponent<Behemoth::MoveComponent>(platform, position);
	registry.AddComponent<Behemoth::ScalingComponent>(platform, scale);
	registry.AddComponent<Behemoth::RotationComponent>(platform, q);

	if (isStatic)
	{
		registry.AddComponent<Behemoth::StaticComponent>(platform);
	}
	else
	{
		registry.AddComponent<Behemoth::RigidBodyComponent>(platform);
	}
	return platform;
}

ECS::EntityHandle PlatformFactory::CreateRockPlatform(ECS::Registry& registry, const BMath::Vector3& position, const BMath::Vector3& scale, const BMath::Quaternion& q, bool isStatic)
{
	bool isRotated = BMath::Quaternion::Equals(q, BMath::Quaternion::Identity());

	ECS::EntityHandle platform = CreatePlatform(
		registry,
		PhysMat::platformPhysicsMats[PhysMat::Rock],
		"plane5.obj",
		"asphalt.jpg",
		"Rock Platform", 
		BMath::Vector2(10.0,10.0),
		isRotated);

	registry.AddComponent<Behemoth::MoveComponent>(platform, position);
	registry.AddComponent<Behemoth::ScalingComponent>(platform, scale);
	registry.AddComponent<Behemoth::RotationComponent>(platform, q, true);

	if (isStatic)
	{
		registry.AddComponent<Behemoth::StaticComponent>(platform);
	}
	else
	{
		registry.AddComponent<Behemoth::RigidBodyComponent>(platform);
	}

	return platform;
}

ECS::EntityHandle PlatformFactory::CreateSandPlatform(ECS::Registry& registry, const BMath::Vector3& position, const BMath::Vector3& scale, const BMath::Quaternion& q, bool isStatic)
{
	bool isRotated = BMath::Quaternion::Equals(q, BMath::Quaternion::Identity());

	ECS::EntityHandle platform = CreatePlatform(
		registry,
		PhysMat::platformPhysicsMats[PhysMat::Sand],
		"plane5.obj",
		"sand.jpg",
		"Sand Platform",
		BMath::Vector2(5.0, 5.0),
		isRotated);

	registry.AddComponent<Behemoth::MoveComponent>(platform, position);
	registry.AddComponent<Behemoth::ScalingComponent>(platform, scale);
	registry.AddComponent<Behemoth::RotationComponent>(platform, q);

	if (isStatic)
	{
		registry.AddComponent<Behemoth::StaticComponent>(platform);
	}
	else
	{
		registry.AddComponent<Behemoth::RigidBodyComponent>(platform);
	}

	return platform;
}

