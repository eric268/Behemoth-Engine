project "TestProject"
    kind "StaticLib"
    language "C++"
        cppdialect "C++20"
        staticruntime "off"
        floatingpoint "fast"
        
        targetdir ("%{wks.location}/Build/bin/%{cfg.buildcfg}/%{cfg.system}/TestProject")
        objdir    ("%{wks.location}/Build/obj/%{cfg.buildcfg}/%{cfg.system}/TestProject")
        
    files { "**.h", "**.hpp", "**.cpp" }
        
    includedirs { "%{wks.location}/Source/Engine/BehemothCore", "%{wks.location}/Source/", "%{wks.location}/Source/Games/TestProject"}
        
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
