#pragma once

#include "BehemothEngine.h"

struct PlayerComponent;
struct PCComponent;

class PCSystem
{
public:
	void Run(const float deltaTime, ECS::Registry& registry);

private:
	void IncreasePower(const float deltaTime, PlayerComponent* playerComp, PCComponent* pcComp);
	void DecreasePower(const float deltaTime, PlayerComponent* playerComp, PCComponent* pcComp);

	void Fire(ECS::Registry& registry, const ECS::EntityHandle& handle, PlayerComponent* playerComp, PCComponent* pcComp);
	void Look(const float deltaTime, ECS::Registry& registry, PlayerComponent* playerComp, PCComponent* pcComp);
	void Aim(const float deltaTime, ECS::Registry& registry, PlayerComponent* playerComp, PCComponent* pcComp);

	void SetArrowMeshVisibility(ECS::Registry& registry, PlayerComponent* playerComp, bool isVisible);
	void RotateMeshWhileMoving(ECS::Registry& registry, const ECS::EntityHandle& entity, PlayerComponent* playerComp);
	void CheckPlayerLanded(ECS::Registry& registry, const ECS::EntityHandle& entity, PlayerComponent* playerComp);
};

