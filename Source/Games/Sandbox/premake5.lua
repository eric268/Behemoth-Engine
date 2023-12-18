project "Sandbox"
   kind "StaticLib"
   language "C++"
   cppdialect    "C++20"
   staticruntime "off"
   floatingpoint "fast"


   targetdir (outputdir .. "%{prj.name}/")
   objdir    (objectdir .. "%{prj.name}/")

   files { "**.h","**.hpp", "**.cpp" }

      -- Specify library directories based on the build configuration and StaticLib's location
      libdirs (outputdir .. "Engine/")
   
      -- Link against the StaticLib project
      links { "Engine" }

      includedirs { "%{wks.location}/Source/Engine/BehemothCore", "%{wks.location}/Source/"}
   
   -- Specify library directories based on the build configuration
   filter "system:windows"
   systemversion "latest"
   defines {}

-- Specify library directories based on the build configuration
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