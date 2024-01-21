#include "pch.h"
#include "GoalObject.h"
#include "Factories/GameObjectFactory.h"

using namespace Behemoth;

ECS::EntityHandle GoalObject::CreateGoal(ECS::Registry& registry, const BMath::Vector3& location, const BMath::Vector3& scale, float rotationAngle)
{
    GameObjectFactory gameObjectFactory{};
    ECS::EntityHandle goalHandle = gameObjectFactory.CreateGameObject(registry, "monkey.obj", "rock.png", "Goal Game Object");
    registry.AddComponent<MoveComponent>(goalHandle, location);
    registry.AddComponent<RotationComponent>(goalHandle, BMath::Quaternion(DEGREE_TO_RAD(rotationAngle), BMath::Vector3(0, 1, 0)));
    registry.AddComponent<StaticComponent>(goalHandle);
    registry.AddComponent<SphereColliderComponent>(goalHandle, 0.75f);
    registry.AddComponent<ScalingComponent>(goalHandle, scale);

    return goalHandle;
}
