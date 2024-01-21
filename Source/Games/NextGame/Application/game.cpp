#include "pch.h"
#include "World/World.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/HoleTwoScene.h"
#include "Scenes/HoleOneScene.h"
#include "Scenes/HoleThreeScene.h"
#include "Scenes/GameOverScene.h"
            
void CreateApplication()
{
    Behemoth::World::GetInstance().Init();
    Behemoth::Scene* mainScene = new HoleTwoScene();
    Behemoth::World::GetInstance().ChangeScene(mainScene);
}