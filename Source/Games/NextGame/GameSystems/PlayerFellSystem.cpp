#include "pch.h"
#include "PlayerFellSystem.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Components/Components.h"


using namespace Behemoth;
void PlayerFellSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	for (const auto& [
		entity, 
			playerComponent, 
			playerFellComponent] : registry.Get<PlayerComponent, PlayerFellComponent>())
	{
 		if (TransformComponent* springArmTransform = registry.GetComponent<TransformComponent>(playerComponent->cameraSpringArm))
 		{
 			springArmTransform->localPosition = springArmTransform->worldPosition;
 			springArmTransform->localTransform._41 = springArmTransform->localPosition.x;
 			springArmTransform->localTransform._42 = springArmTransform->localPosition.y;
 			springArmTransform->localTransform._43 = springArmTransform->localPosition.z;
 		}

		registry.RemoveComponent<Behemoth::ChildComponent>(playerComponent->cameraSpringArm);

		ECS::EntityHandle timerEntity = registry.CreateEntity("Timer Component");
		registry.AddComponent<Behemoth::TimerComponent>(timerEntity, playerFellComponent->timeUntilRespawn,
			[this, &registry, entity]() { this->RespawnPlayer(registry, entity); });

		// RespawnPlayer(registry, entity);
		registry.RemoveComponent<PlayerFellComponent>(entity);
	}
}

void PlayerFellSystem::RespawnPlayer(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
{
	registry.AddComponent<PlayerRespawnComponent>(entityHandle);
}
