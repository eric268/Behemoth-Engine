#include <iostream>
#include "World/World.h"
#include "Scenes/MainScene.h"
            
    void CreateApplication()
    {
        Behemoth::Scene* mainScene = new MainScene();
        Behemoth::World::GetInstance().ChangeScene(mainScene);
    }
            