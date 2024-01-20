#include "pch.h"
#include "PlayerProjectile.h"
#include "Factories/GameObjectFactory.h"

using namespace Behemoth;

ECS::EntityHandle PlayerProjectile::InitalizePlayer(ECS::Registry& registry)
{
	GameObjectFactory gameObjectFactory{};

	playerHandle = gameObjectFactory.CreateGameObject(registry, "", "", "Player");
	registry.AddComponent<RigidBodyComponent>(playerHandle, false);
	registry.AddComponent<MoveComponent>(playerHandle, BMath::Vector3(0, 10, 10));
	registry.AddComponent<OBBColliderComponent>(playerHandle);

	projectileHandle = registry.CreateEntity("Projectile");
	registry.AddComponent<TransformComponent>(projectileHandle);

	playerMeshHandle = gameObjectFactory.CreateGameObject(registry, "sphere.obj", "rock.png", "Projectile Mesh");

	arrowMeshHandle = gameObjectFactory.CreateGameObject(registry, "arrow.obj", "arrow.jpg", "Arrow icon");
	registry.AddComponent<ScalingComponent>(arrowMeshHandle, BMath::Vector3(0.25));
	registry.AddComponent<MoveComponent>(arrowMeshHandle, BMath::Vector3(0, 0, -3));

	BMath::Quaternion q1 = BMath::Quaternion(DEGREE_TO_RAD(-90.0f), BMath::Vector3(0, 0, 1));
	BMath::Quaternion q2 = BMath::Quaternion(DEGREE_TO_RAD(90), BMath::Vector3(1, 0, 0));
	registry.AddComponent<RotationComponent>(arrowMeshHandle, q1 * q2, true);

	gameObjectFactory.AddChildObject(registry, playerHandle, projectileHandle);
	gameObjectFactory.AddChildObject(registry, projectileHandle, arrowMeshHandle);
	gameObjectFactory.AddChildObject(registry, projectileHandle, playerMeshHandle);

	return playerHandle;
}