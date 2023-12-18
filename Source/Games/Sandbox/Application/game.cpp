#include <iostream>
#include "World/World.h"
#include "Scenes/MainScene.h"


void CreateApplication()
{
	Behemoth::MainScene* mainScene = new MainScene();
	Behemoth::World::GetInstance().ChangeScene(mainScene);
}
