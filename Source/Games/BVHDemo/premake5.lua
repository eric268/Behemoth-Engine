project "BVHDemo"
    kind "StaticLib"
    language "C++"
        cppdialect "C++20"
        staticruntime "off"
        floatingpoint "fast"

        pchheader "pch.h"
        pchsource "%{wks.location}/Source/Games/BVHDemo/pch.cpp"
        
        targetdir ("%{wks.location}/Build/bin/%{cfg.buildcfg}/%{cfg.system}/BVHDemo")
        objdir    ("%{wks.location}/Build/obj/%{cfg.buildcfg}/%{cfg.system}/BVHDemo")
        
    files { "%{wks.location}/Source/Games/BVHDemo/**.h", 
            "%{wks.location}/Source/Games/BVHDemo/**.hpp",
            "%{wks.location}/Source/Games/BVHDemo/**.cpp" }
        
    includedirs { "%{wks.location}/Source/Engine/BehemothCore", "%{wks.location}/Source/", "%{wks.location}/Source/Games/BVHDemo"}
        
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
