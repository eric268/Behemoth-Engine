
function CreateProject(projectName)
    -- Create a directory for the new project
    local projectDir = "../Games/" .. projectName

    -- Create project directory
    os.execute("mkdir \"" .. projectDir .. "\"")
    os.execute("mkdir \"" .. projectDir .. "\\Scenes\"")
    os.execute("mkdir \"" .. projectDir .. "\\Application\"")

    

    local sandboxDir = "../Games/Sandbox/"
    os.execute("xcopy \"" .. sandboxDir .. "Scenes\\\" \"" .. projectDir .. "\\Scenes\\\" /E /I")
    os.execute("xcopy \"" .. sandboxDir .. "Application\\\" \"" .. projectDir .. "\\Application\\\" /E /I")
    
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
        
        targetdir ("]] .. outputdir .. projectName .. [[")
        objdir    ("]] .. objectdir .. projectName .. [[")
        
    files { "**.h", "**.hpp", "**.cpp" }
        
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