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
	else if (result < 4)
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
			// Player respawn is already being processed
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
	oobTrigger = registry.CreateEntity("Out of bounds trigger");
	registry.AddComponent<Behemoth::TransformComponent>(oobTrigger);
	registry.AddComponent<Behemoth::AABBColliderComponent>(oobTrigger, BMath::Vector3(1.0f), true, true);
	registry.AddComponent<Behemoth::StaticComponent>(oobTrigger);
	registry.AddComponent<Behemoth::ScalingComponent>(oobTrigger, BMath::Vector3(1000, 10.0f, 1000.0));
	registry.AddComponent<Behemoth::MoveComponent>(oobTrigger, BMath::Vector3(0, -20, 10.0f));
	registry.AddComponent<Behemoth::BVHColliderComponent>(oobTrigger);

	return oobTrigger;
}

ECS::EntityHandle GameScene::CreateGoalCollider(ECS::Registry& registry, BMath::Vector3 offset, BMath::Vector3 scale)
{
	ECS::EntityHandle collider1 = registry.CreateEntity("Goal Collider");
	registry.AddComponent<Behemoth::TransformComponent>(collider1);
	registry.AddComponent<Behemoth::SphereColliderComponent>(collider1);
	registry.AddComponent<Behemoth::ScalingComponent>(collider1, scale);
	registry.AddComponent<Behemoth::MoveComponent>(collider1, offset);
	registry.AddComponent<Behemoth::MeshInitalizeComponent>(collider1);
	registry.AddComponent<Behemoth::WireframeComponent>(collider1, "sphere.obj", BMath::Vector3(1.0f));
	registry.AddComponent<Behemoth::RotationComponent>(collider1);
	registry.AddComponent<Behemoth::StaticComponent>(collider1);
	return collider1;
}

ECS::EntityHandle GameScene::CreateGoalObject(ECS::Registry& registry, const BMath::Vector3& location, const BMath::Vector3& scale, float rotationAngle)
{
	ECS::EntityHandle goalHandle = Behemoth::GameObjectFactory::CreateGameObject(registry, "monkey.obj", "rock.png", "Goal Game Object");
	registry.AddComponent<Behemoth::MoveComponent>(goalHandle, location);
	registry.AddComponent<Behemoth::RotationComponent>(goalHandle, BMath::Quaternion(DEGREE_TO_RAD(rotationAngle), BMath::Vector3(0, 1, 0)));
	registry.AddComponent<Behemoth::StaticComponent>(goalHandle);
	registry.AddComponent<Behemoth::ScalingComponent>(goalHandle, scale);
	registry.AddComponent<Behemoth::SphereColliderComponent>(goalHandle);
	registry.AddComponent<Behemoth::WireframeComponent>(goalHandle, "sphere.obj");

// 	ECS::EntityHandle collider1 = CreateGoalCollider(registry, BMath::Vector3(0, 0.75f, -0.25f), BMath::Vector3(0.85f));
// 	ECS::EntityHandle collider2 = CreateGoalCollider(registry, BMath::Vector3(0, -2, 2), BMath::Vector3(0.4f));
// 	ECS::EntityHandle collider3 = CreateGoalCollider(registry, BMath::Vector3(-3.25, 0.5f, -1.0f), BMath::Vector3(0.35f));
// 	ECS::EntityHandle collider4 = CreateGoalCollider(registry, BMath::Vector3(3.25, 0.5f, -1.0f), BMath::Vector3(0.35f));
// 
// 	Behemoth::GameObjectFactory::AddChildObject(registry, goalHandle, collider1);
// 	Behemoth::GameObjectFactory::AddChildObject(registry, goalHandle, collider2);
// 	Behemoth::GameObjectFactory::AddChildObject(registry, goalHandle, collider3);
// 	Behemoth::GameObjectFactory::AddChildObject(registry, goalHandle, collider4);

	return goalHandle;
}