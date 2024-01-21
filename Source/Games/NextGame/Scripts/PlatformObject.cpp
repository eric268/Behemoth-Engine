#include "pch.h"
#include "PlatformObject.h"
#include "Factories/GameObjectFactory.h"
#include "PlatformPhysMats.h"

using namespace Behemoth;

ECS::EntityHandle PlatformObject::CreatePlatform(ECS::Registry& registry, const PhysicsMaterial& physicsMaterial, const std::string& modelPath, const std::string& texturePath, const std::string& name, const BMath::Vector2& uvScale)
{
	ECS::EntityHandle platform = GameObjectFactory::CreateGameObject(registry, modelPath, texturePath, name, uvScale);
	OBBColliderComponent* colliderComp = registry.AddComponent<OBBColliderComponent>(platform, BMath::Vector3(1.1f));

	if (!colliderComp)
	{
		LOGMESSAGE(Error, "Unable to add OBB collider to platform");
		return platform;
	}

	colliderComp->physicsMaterial = physicsMaterial;
	return platform;
}

ECS::EntityHandle PlatformObject::CreateGrassPlatform(ECS::Registry& registry, const BMath::Vector3& position, const BMath::Vector3& scale, const BMath::Quaternion& q, bool isStatic)
{
	ECS::EntityHandle platform = CreatePlatform(
		registry,
		PhysMat::platformPhysicsMats[PhysMat::Grass],
		"plane.obj",
		"grass.png",
		"Grass Platform",
		BMath::Vector2(8,8));

	registry.AddComponent<MoveComponent>(platform, position);
	registry.AddComponent<ScalingComponent>(platform, scale);
	registry.AddComponent<RotationComponent>(platform, q);

	if (isStatic)
	{
		registry.AddComponent<StaticComponent>(platform);
	}
	return platform;
}

ECS::EntityHandle PlatformObject::CreateRockPlatform(ECS::Registry& registry, const BMath::Vector3& position, const BMath::Vector3& scale, const BMath::Quaternion& q, bool isStatic)
{
	ECS::EntityHandle platform = CreatePlatform(
		registry,
		PhysMat::platformPhysicsMats[PhysMat::Rock],
		"plane.obj",
		"asphalt.jpg",
		"Rock Platform", 
		BMath::Vector2(10.0,10.0));

	registry.AddComponent<MoveComponent>(platform, position);
	registry.AddComponent<ScalingComponent>(platform, scale);
	registry.AddComponent<RotationComponent>(platform, q, true);

	if (isStatic)
	{
		registry.AddComponent<StaticComponent>(platform);
	}
	return platform;
}

ECS::EntityHandle PlatformObject::CreateSandPlatform(ECS::Registry& registry, const BMath::Vector3& position, const BMath::Vector3& scale, const BMath::Quaternion& q, bool isStatic)
{
	ECS::EntityHandle platform = CreatePlatform(
		registry,
		PhysMat::platformPhysicsMats[PhysMat::Sand],
		"plane.obj",
		"sand.jpg",
		"Sand Platform",
		BMath::Vector2(5.0, 5.0));

	registry.AddComponent<MoveComponent>(platform, position);
	registry.AddComponent<ScalingComponent>(platform, scale);
	registry.AddComponent<RotationComponent>(platform, q);

	if (isStatic)
	{
		registry.AddComponent<StaticComponent>(platform);
	}
	return platform;
}

