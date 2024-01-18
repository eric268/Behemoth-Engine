#include "pch.h"
#include "World/World.h"
#include "Scenes/MainScene.h"
            
void CreateApplication()
{
    Behemoth::World::GetInstance().Init();
    Behemoth::Scene* mainScene = new MainScene();
    Behemoth::World::GetInstance().ChangeScene(mainScene);
}
            