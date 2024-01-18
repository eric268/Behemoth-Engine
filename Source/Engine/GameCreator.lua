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
    local sandboxDir = "../Games/Sandbox/"

    -- Function to check if a directory exists
    local function directoryExists(directory)
        return os.execute("if exist \"" .. directory .. "\" (exit 0) else (exit 1)")
    end

    -- Check if the project directory exists
    if directoryExists(projectDir) then
        print("Error - Project with this name already exists.")
        return false
    end

    -- Create project directory
    os.execute("mkdir \"" .. projectDir .. "\"")

    local function copyDirectory(source, destination)
        local command = "xcopy /E /I /Y \"" .. source .. "\" \"" .. destination .. "\""
        local result = os.execute(command)
        if result ~= true then
            print("Failed to copy directory from " .. source .. " to " .. destination)
        end
    end

    local function copyFile(sourceFile, destinationFile)
        os.execute("copy /Y \"" .. sourceFile .. "\" \"" .. destinationFile .. "\"")
    end

    copyFile(sandboxDir .. "\\pch.h", projectDir .. "\\pch.h")
    copyFile(sandboxDir .. "\\pch.cpp", projectDir .. "\\pch.cpp")

    copyDirectory(sandboxDir .. "\\Scenes\\", projectDir .. "\\Scenes")
    copyDirectory(sandboxDir .. "\\Application\\", projectDir .. "\\Application")

    CreatePremakeConfig(projectName)

    -- End of CreateProject function
end

function CreateFile(destination, filename, contents)
    local file = io.open(destination .. "\\" .. filename, "w")
    if file then
        file:write(contents)
        file:close()
    else
        print("Failed to create " .. filename .. " in " .. destination)
    end
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