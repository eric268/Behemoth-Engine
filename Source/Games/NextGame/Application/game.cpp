#include "pch.h"
#include "World/World.h"
#include "Scenes/HoleOneScene.h"
            
void CreateApplication()
{
    Behemoth::World::GetInstance().Init();
    Behemoth::Scene* mainScene = new HoleOneScene();
    Behemoth::World::GetInstance().ChangeScene(mainScene);
}