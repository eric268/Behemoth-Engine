project "TestProject123"
    kind "StaticLib"
    language "C++"
        cppdialect "C++20"
        staticruntime "off"
        floatingpoint "fast"

        pchheader "pch.h"
        pchsource "%{wks.location}/Source/Games/TestProject123/pch.cpp"
        
        targetdir ("%{wks.location}/Build/bin/%{cfg.buildcfg}/%{cfg.system}/TestProject123")
        objdir    ("%{wks.location}/Build/obj/%{cfg.buildcfg}/%{cfg.system}/TestProject123")
        
    files { "%{wks.location}/Source/Games/TestProject123/**.h", 
            "%{wks.location}/Source/Games/TestProject123/**.hpp",
            "%{wks.location}/Source/Games/TestProject123/**.cpp" }
        
    includedirs { "%{wks.location}/Source/Engine/BehemothCore", "%{wks.location}/Source/", "%{wks.location}/Source/Games/TestProject123"}
        
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
