#include "pch.h"
#include "PlayerFallSystem.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Components/Components.h"

void PlayerFallSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	for (const auto& [
		entityHandle, 
			playerComp, 
			playerFellComp] : registry.Get<PlayerComponent, PlayerFallComponent>())
	{
 		if (Behemoth::TransformComponent* springArmTransformComp = registry.GetComponent<Behemoth::TransformComponent>(playerComp->cameraSpringArm))
 		{
 			springArmTransformComp->localPosition = springArmTransformComp->worldPosition;
 			springArmTransformComp->localTransform._41 = springArmTransformComp->localPosition.x;
 			springArmTransformComp->localTransform._42 = springArmTransformComp->localPosition.y;
 			springArmTransformComp->localTransform._43 = springArmTransformComp->localPosition.z;
 		}

		playerComp->isRespawning = true;
		registry.RemoveComponent<Behemoth::ChildComponent>(playerComp->cameraSpringArm);

		ECS::EntityHandle timerEntityHandle = registry.CreateEntity("Timer Component");
		registry.AddComponent<Behemoth::TimerComponent>(timerEntityHandle, playerFellComp->timeUntilRespawn,
			[this, &registry, entityHandle]() { this->RespawnPlayer(registry, entityHandle); });

		registry.RemoveComponent<PlayerFallComponent>(entityHandle);
	}
}

void PlayerFallSystem::RespawnPlayer(ECS::Registry& registry, const ECS::EntityHandle& entityHandle)
{
	registry.AddComponent<PlayerRespawnComponent>(entityHandle);
}
