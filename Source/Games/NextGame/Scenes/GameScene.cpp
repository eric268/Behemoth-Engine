#include "pch.h"
#include "GameScene.h"
#include "Components/UIComponents.h"
#include "GameComponents/Player/PlayerComponent.h"
#include "Scripts/PlayerScore.h"



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
