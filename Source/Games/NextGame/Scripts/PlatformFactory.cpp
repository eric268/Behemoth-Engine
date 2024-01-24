#include "pch.h"
#include "PlatformFactory.h"
#include "Factories/GameObjectFactory.h"
#include "Components/RenderComponents.h"
#include "PlatformPhysMats.h"
#include "Math/Quaternion.h"

using namespace Behemoth;

ECS::EntityHandle PlatformFactory::CreatePlatform(
	ECS::Registry& registry,
	const PhysicsMaterial& physicsMaterial,
	const std::string& modelPath,
	const std::string& texturePath,
	const std::string& name,
	const BMath::Vector2& uvScale,
	bool isRotated)
{
	ECS::EntityHandle platform = GameObjectFactory::CreateGameObject(registry, modelPath, texturePath, name, uvScale);

	if (isRotated)
	{
		OBBColliderComponent* colliderComp = registry.AddComponent<OBBColliderComponent>(platform, BMath::Vector3(1.0f));
		if (colliderComp)
		{
			colliderComp->physicsMaterial = physicsMaterial;
		}
	}
	else
	{
		AABBColliderComponent* colliderComp = registry.AddComponent<AABBColliderComponent>(platform, BMath::Vector3(1.0f));
		if (colliderComp)
		{
			colliderComp->physicsMaterial = physicsMaterial;
		}
	}

	return platform;
}

ECS::EntityHandle PlatformFactory::CreateGrassPlatform(ECS::Registry& registry, const BMath::Vector3& position, const BMath::Vector3& scale, const BMath::Quaternion& q, bool isStatic)
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

	registry.AddComponent<MoveComponent>(platform, position);
	registry.AddComponent<ScalingComponent>(platform, scale);
	registry.AddComponent<RotationComponent>(platform, q);
	registry.AddComponent<Behemoth::WireframeComponent>(platform, "plane5.obj");

	if (isStatic)
	{
		registry.AddComponent<StaticComponent>(platform);
	}
	else
	{
		registry.AddComponent<RigidBodyComponent>(platform);
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

	registry.AddComponent<MoveComponent>(platform, position);
	registry.AddComponent<ScalingComponent>(platform, scale);
	registry.AddComponent<RotationComponent>(platform, q, true);

	if (isStatic)
	{
		registry.AddComponent<StaticComponent>(platform);
	}
	else
	{
		registry.AddComponent<RigidBodyComponent>(platform);
	}

	return platform;
}

ECS::EntityHandle PlatformFactory::CreateSandPlatform(ECS::Registry& registry, const BMath::Vector3& position, const BMath::Vector3& scale, const BMath::Quaternion& q, bool isStatic)
{
	bool isRotated = BMath::Quaternion::Equals(q, BMath::Quaternion::Identity());

	ECS::EntityHandle platform = CreatePlatform(
		registry,
		PhysMat::platformPhysicsMats[PhysMat::Sand],
		"cube20.obj",
		"sand.jpg",
		"Sand Platform",
		BMath::Vector2(5.0, 5.0),
		isRotated);

	registry.AddComponent<MoveComponent>(platform, position);
	registry.AddComponent<ScalingComponent>(platform, scale);
	registry.AddComponent<RotationComponent>(platform, q);

	if (isStatic)
	{
		registry.AddComponent<StaticComponent>(platform);
	}
	else
	{
		registry.AddComponent<RigidBodyComponent>(platform);
	}

	return platform;
}

