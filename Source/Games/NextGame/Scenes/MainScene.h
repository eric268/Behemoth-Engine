#pragma once
            
#include "World/Scene.h"
#include "ECS/Entity.h" 
            
class MainScene : public Behemoth::Scene
{
public:
        MainScene();
        void Init() override;
        void Update(const float deltaTime) override;
        void Shutdown() override;
        
        ECS::Entity pointLight;
private:
            
};
            