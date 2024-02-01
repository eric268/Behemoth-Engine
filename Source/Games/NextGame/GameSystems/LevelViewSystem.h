#pragma once
#include "BehemothEngine.h"

struct LevelViewComponent;
struct ViewControllerComponent;

class LevelViewSystem
{
public:
	void Run(const float deltaTime, ECS::Registry& registry);

private:
	void Look(
		const float deltaTime,
		ECS::Registry& registry,
		const ECS::EntityHandle& entity,
		LevelViewComponent* playerComponent,
		ViewControllerComponent* pcComponent);
};

