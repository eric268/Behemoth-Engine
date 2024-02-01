#pragma once
#include "BehemothEngine.h"

struct GoalViewComponent;
struct ViewControllerComponent;

class goalViewSystem
{
public:
	void Run(const float deltaTime, ECS::Registry& registry);

private:
	void Look(
		const float deltaTime,
		ECS::Registry& registry,
		const ECS::EntityHandle& entity,
		GoalViewComponent* playerComponent,
		ViewControllerComponent* pcComponent);
};

