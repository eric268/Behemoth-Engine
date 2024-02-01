#include "pch.h"
#include "World/World.h"
#include "Scenes/MainMenuScene.h"
            
void CreateApplication()
{
    Behemoth::World::GetInstance().InitializeWorld();
    Behemoth::Scene* mainScene = new MainMenuScene();
    Behemoth::World::GetInstance().ChangeScene(mainScene);
}