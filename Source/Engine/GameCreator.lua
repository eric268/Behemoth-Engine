function createFile(path, content)
    local file = io.open(path, "w")
    if file then
        file:write(content)
        file:close()
    else
        print("Error: Unable to create file at " .. path)
    end
end

function CreateProject(projectName)
    -- Create a directory for the new project
    local projectDir = "../Games/" .. projectName

    -- Create project directory
    os.execute("mkdir \"" .. projectDir .. "\"")
    os.execute("mkdir \"" .. projectDir .. "\\Scenes\"")
    os.execute("mkdir \"" .. projectDir .. "\\Application\"")

        -- Create MainScene.h
        local mainSceneHeader = [[
#pragma once
            
#include "World/Scene.h"
            
class MainScene : public Behemoth::Scene
    {
    public:
        void Init() override;
        void Update(const float deltaTime) override;
        void Shutdown() override;
            
    private:
            
    };
            ]]
            createFile(projectDir .. "\\Scenes\\MainScene.h", mainSceneHeader)
            
                -- Create MainScene.cpp
            local mainSceneSource = [[
#include "MainScene.h"
#include "Factories/CameraFactory.h"
#include "Factories/LightFactories.h"
#include "Components/Components.h"
            
void MainScene::Init()
    {
        Behemoth::CameraFactory cameraFactory{};
        cameraFactory.CreateCamera(registry, true, "Main Camera");
            
        Behemoth::DirectionalLightFactory dirLightFactory{};
        dirLightFactory.CreateDirectionalLight(registry);
            
        Behemoth::PointLightFactory pointLightFactory{};
        pointLightFactory.CreatePointLight(registry, "Point Light 1");
             
        for (int i = -1; i < 2; i++)
        {
            ECS::Entity e1 = registry.CreateEntity("Cube 1");
            registry.AddComponent<Behemoth::MeshComponent>(e1, "monkey.obj", "diamond.png");
            registry.AddComponent<Behemoth::TransformComponent>(e1);
            registry.AddComponent<Behemoth::MeshInitalizeComponent>(e1);
            registry.AddComponent<Behemoth::RotationComponent>(e1, i + 1, 1.0f);
            registry.AddComponent<Behemoth::MovementComponent>(e1, Math::Vector3(-3.0f * i, 0.0f, -5.0f));
            registry.AddComponent<Behemoth::ScalingComponent>(e1, Math::Vector3(1.0f, 1.0f, 1.0f));
            registry.AddComponent<Behemoth::BoundingVolumeComponent>(e1, 1.5f, false);
        }
    }
            
    void MainScene::Update(const float deltaTime)
    {
            
            
    }
            
    void MainScene::Shutdown()
    {
            
    }
            ]]
            createFile(projectDir .. "\\Scenes\\MainScene.cpp", mainSceneSource)
            
                -- Create game.cpp
            local gameSource = [[
#include <iostream>
#include "World/World.h"
#include "Scenes/MainScene.h"
            
    void CreateApplication()
    {
        Behemoth::Scene* mainScene = new MainScene();
        Behemoth::World::GetInstance().ChangeScene(mainScene);
    }
            ]]
                createFile(projectDir .. "\\Application\\game.cpp", gameSource)
    
    local pchHeader = [[
#pragma once
    ]]
    createFile(projectDir .. "\\pch.h", pchHeader)

    local pchSource = [[
#include "pch.h"
    ]]
    createFile(projectDir .. "\\pch.cpp", pchSource)
    
    CreatePremakeConfig(projectName)
end


function CreatePremakeConfig(projectName)

    local premakeConfig = [[
project "]] .. projectName .. [["
    kind "StaticLib"
    language "C++"
        cppdialect "C++20"
        staticruntime "off"
        floatingpoint "fast"

        pchheader "pch.h"
        pchsource "%{wks.location}/Source/Games/]] .. projectName .. [[/pch.cpp"
        
        targetdir ("]] .. outputdir .. projectName .. [[")
        objdir    ("]] .. objectdir .. projectName .. [[")
        
    files { "%{wks.location}/Source/Games/]] .. projectName .. [[/**.h", 
            "%{wks.location}/Source/Games/]] .. projectName .. [[/**.hpp",
            "%{wks.location}/Source/Games/]] .. projectName .. [[/**.cpp" }
        
    includedirs { "%{wks.location}/Source/Engine/BehemothCore", "%{wks.location}/Source/", "%{wks.location}/Source/Games/]] .. projectName .. [["}
        
    links { "Engine" }
        
    filter "system:windows"
        systemversion "latest"
        defines {}
        
    filter "configurations:Debug"
        libdirs {"Build/Debug"}
        defines { "DEBUG" }
        symbols "On"
        
    filter "configurations:Release"
        libdirs {"Build/Release"}
        defines { "RELEASE" }
        optimize "On"
        symbols "On"
        
    filter "configurations:Dist"
        libdirs {"Build/Dist"}
        defines {"DIST"}
        runtime "Release"
        optimize "On"
        symbols "Off"
]]
        
    -- Write the premake configuration to a file in the new project directory
    local premakeFile = io.open("../Games/" .. projectName .. "/premake5.lua", "w")
    premakeFile:write(premakeConfig)
    premakeFile:close()
end