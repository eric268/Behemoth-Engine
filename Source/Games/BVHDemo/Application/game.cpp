#include "pch.h"
#include "World/World.h"
#include "Scenes/DemoScene.h"

void CreateApplication()
{
	Behemoth::World::GetInstance().InitializeWorld();
	Behemoth::Scene* mainScene = new DemoScene();
	Behemoth::World::GetInstance().ChangeScene(mainScene);
}
