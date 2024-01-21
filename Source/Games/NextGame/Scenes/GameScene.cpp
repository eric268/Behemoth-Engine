#include "pch.h"
#include "GameScene.h"
#include "Components/UIComponents.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Scripts/PlayerScore.h"
#include "Factories/GameObjectFactory.h"
#include "Components/RenderComponents.h"


std::unordered_map<int, std::string> GameScene::holeResultsText =
{
	{-3, "Albatross"},
	{-2, "Eagle"},
	{-1, "Birdie"},
	{0, "Par"},
	{1, "Bogey"},
	{2, "Double Bogey"},
	{3, "Triple Bogey"}
};


std::string GameScene::GetHoleResultText(int numStrokes, int par)
{
	int result = numStrokes - par;

	if (numStrokes == 1)
	{
		return std::string("Hole in One! Well Done!");
	}
	else if (numStrokes < 4)
	{
		return holeResultsText[result];
	}
	else
	{
		return std::string("+" + std::to_string(result));
	}
}

void GameScene::CheckOutOfBound(ECS::Registry& registry, const ECS::EntityHandle& playerHandle, const ECS::EntityHandle& oobHandle)
{
	if (Behemoth::TriggerDataComponent* triggerData = registry.GetComponent<Behemoth::TriggerDataComponent>(playerHandle))
	{
		PlayerComponent* playerComponent = registry.GetComponent<PlayerComponent>(playerHandle);
		if (!playerComponent)
		{
			LOGMESSAGE(Error, "Null player component found");
			return;
		}

		if (playerComponent->isRespawning)
		{
			// Player fell is already being processed
			return;
		}

		for (const auto& d : triggerData->data)
		{
			if (d.otherHandle.ID == oobHandle.ID)
			{
				registry.AddComponent<PlayerFellComponent>(playerHandle, 3.0f);
			}
		}
	}
}

bool GameScene::CheckLevelComplete(ECS::Registry& registry, ECS::EntityHandle& playerHandle)
{
	if (Behemoth::CollisionDataComponent* collisionData = registry.GetComponent<Behemoth::CollisionDataComponent>(playerHandle))
	{
		for (const auto& d : collisionData->data)
		{
			if (d.otherHandle.ID == goalObject.ID)
			{
				PlayerComponent* playerComponent = registry.GetComponent<PlayerComponent>(playerHandle);
				if (!playerComponent)
				{
					LOGMESSAGE(Error, "Unable to find player component");
					return false;
				}
				// Level complete process already happening
				if (playerComponent->levelComplete)
				{
					return false;
				}

				LOGMESSAGE(General, "Goal Reached!");
				playerComponent->levelComplete = true;

				return true;
			}
		}
	}

	return false;
}

void GameScene::OnHoleComplete(ECS::Registry& registry, ECS::EntityHandle& playerHandle, int par)
{
	PlayerComponent* playerComponent = registry.GetComponent<PlayerComponent>(playerHandle);
	if (!playerComponent)
	{
		LOGMESSAGE(Error, "Unable to find player component");
		return;
	}

	PlayerScore::AddScore(par, playerComponent->strokesUsed);

	levelCompleteText = registry.CreateEntity("Level complete text");
	registry.AddComponent<Behemoth::TextComponent>(levelCompleteText, GetHoleResultText(playerComponent->strokesUsed, par), BMath::Vector2(0, 0.5f));

	if (Behemoth::VelocityComponent* velocity = registry.GetComponent<Behemoth::VelocityComponent>(playerCharacter))
	{
		velocity->velocity = BMath::Vector3::Zero();
	}

	if (Behemoth::RigidBodyComponent* rigidBodyComponent = registry.GetComponent<Behemoth::RigidBodyComponent>(playerCharacter))
	{
		rigidBodyComponent->affectedByGravity = false;
	}
}

ECS::EntityHandle GameScene::CreateOOBEntity(ECS::Registry& registry)
{
	bottomOOBTrigger = Behemoth::GameObjectFactory::CreateGameObject(registry, "cube.obj", "rock.png", "Ground entity", { 8,8 });

	registry.AddComponent<Behemoth::OBBColliderComponent>(bottomOOBTrigger, BMath::Vector3(1.0f), true);
	registry.AddComponent<Behemoth::StaticComponent>(bottomOOBTrigger);
	registry.AddComponent<Behemoth::ScalingComponent>(bottomOOBTrigger, BMath::Vector3(1000, 10.0f, 1000.0));
	registry.AddComponent<Behemoth::MoveComponent>(bottomOOBTrigger, BMath::Vector3(0, -20, 10.0f));

	if (Behemoth::MeshComponent* mesh = registry.GetComponent<Behemoth::MeshComponent>(bottomOOBTrigger))
	{
		mesh->isVisible = false;
	}

	return bottomOOBTrigger;
}

ECS::EntityHandle  GameScene::CreateGoalObject(ECS::Registry& registry, const BMath::Vector3& location, const BMath::Vector3& scale, float rotationAngle)
{
	Behemoth::GameObjectFactory gameObjectFactory{};
	ECS::EntityHandle goalHandle = gameObjectFactory.CreateGameObject(registry, "monkey.obj", "rock.png", "Goal Game Object");
	registry.AddComponent<Behemoth::MoveComponent>(goalHandle, location);
	registry.AddComponent<Behemoth::RotationComponent>(goalHandle, BMath::Quaternion(DEGREE_TO_RAD(rotationAngle), BMath::Vector3(0, 1, 0)));
	registry.AddComponent<Behemoth::StaticComponent>(goalHandle);
	registry.AddComponent<Behemoth::SphereColliderComponent>(goalHandle, 0.9f);
	registry.AddComponent<Behemoth::ScalingComponent>(goalHandle, scale);

	return goalHandle;
}