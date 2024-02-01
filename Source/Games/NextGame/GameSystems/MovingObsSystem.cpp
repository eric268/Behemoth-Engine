#include "pch.h"
#include "MovingObsSystem.h"
#include "ECS/Entity.h"
#include "ECS/Registry.h"
#include "Components/PhysicsComponents.h"
#include "GameComponents/Obstacles/MovingObsComponent.h"


void MovingObsSystem::Run(const float deltaTime, ECS::Registry& registry)
{
	for (const auto& [
		entityHandle, 
			movingObsComp, 
			velocityComp] : registry.Get<MovingObsComponent, Behemoth::VelocityComponent>())
	{
		if (movingObsComp->isActive)
		{
			float deltaDistance = movingObsComp->movementSpeed * deltaTime;

			if (movingObsComp->movementCounter + deltaDistance > movingObsComp->maxDistance)
			{
				movingObsComp->dir *= -1.0f;
				movingObsComp->movementCounter = 0.0f;
			}

			movingObsComp->movementCounter += deltaDistance;
			velocityComp->velocity = movingObsComp->movementAxis * movingObsComp->dir;
		}
	}
}
