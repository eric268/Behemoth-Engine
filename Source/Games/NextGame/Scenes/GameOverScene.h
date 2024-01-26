#pragma once

#include "BehemothEngine.h"

class GameOverScene : public Behemoth::Scene
{
public:
	using Super = Behemoth::Scene;

	GameOverScene();
	void Initialize() override;
	void Update(const float deltaTime) override;
	void OnEvent(Behemoth::Event& e) override;
	void Shutdown() override;
	ECS::Registry& GetRegistry() override { return registry; }

private:
	std::string GetFinalScoreText();
	ECS::EntityHandle finalScoreUIHandle;
	ECS::EntityHandle controlsUIHandle;
	ECS::EntityHandle mainCameraHandle;
};

