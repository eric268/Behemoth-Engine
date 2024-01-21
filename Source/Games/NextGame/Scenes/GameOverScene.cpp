#include "pch.h"
#include "GameOverScene.h"
#include "Input/Input.h"
#include "World/World.h"
#include "MainMenuScene.h"
#include "Components/UIComponents.h"
#include "Scripts/PlayerScore.h"

#include "Factories/CameraFactory.h"

GameOverScene::GameOverScene()
{
	Behemoth::CameraFactory cameraFactory{};
	mainCameraHandle = cameraFactory.CreateCamera(registry, true, "Main Camera");
	registry.AddComponent<Behemoth::MoveComponent>(mainCameraHandle, BMath::Vector3(0.0f, 0, 0));

	finalScoreUIHandle = registry.CreateEntity("Final score text");
	registry.AddComponent<Behemoth::TextComponent>(finalScoreUIHandle, GetFinalScoreText(), BMath::Vector2(-0.3, 0.4));

	controlsUIHandle = registry.CreateEntity("Final score text");
	std::string gameOverInstructionsText = "Press [X] on controller or [Spacebar] to return to Main Menu";
	registry.AddComponent<Behemoth::TextComponent>(controlsUIHandle, gameOverInstructionsText, BMath::Vector2(-0.4, 0.3));
}

void GameOverScene::Initalize()
{

}

void GameOverScene::Update(const float deltaTime)
{
	if (Behemoth::Input::IsControllerKeyDown(Behemoth::ControllerCode::CC_X) || Behemoth::Input::IsKeyDown(Behemoth::KeyCode::KC_Space))
	{
		Behemoth::World::GetInstance().ChangeScene(new MainMenuScene());
	}
}

void GameOverScene::OnEvent(Behemoth::Event& e)
{

}

void GameOverScene::InitalizeSystems()
{

}

void GameOverScene::Shutdown()
{

}

std::string GameOverScene::GetFinalScoreText()
{
	if (PlayerScore::GetScore() == 0)
	{
		return std::string("Well done! You shot even with the course par!");
	}

	std::string result = (PlayerScore::GetScore() > 0) ? " over " : " under ";
	return std::string("Well Done! You shot " + std::to_string(PlayerScore::GetScore()) + result + " par!");
}