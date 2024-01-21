#pragma once
#include "BehemothEngine.h"

#include "GameComponents/LevelViewComponent.h"
#include "GameComponents/Player/PCComponent.h"

class LevelViewSystem
{
public:
	void Run(const float deltaTime, ECS::Registry& registry);

private:
	void Look(const float deltaTime, ECS::Registry& registry, ECS::EntityHandle entity, LevelViewComponent* playerComponent, ViewControllerComponent* pcComponent);
	void Zoom(const float deltaTime, ECS::Registry& registry, ECS::EntityHandle, LevelViewComponent* playerComponent, ViewControllerComponent* pcComponent);
};

