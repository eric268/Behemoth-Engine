project "BehemothTests"
   kind "ConsoleApp"
   language "C++"
   cppdialect    "C++20"
   staticruntime "off"
   floatingpoint "fast"
   rtti          "on"

   nuget { "Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn:1.8.1.4" }
   
   flags {
       "FatalWarnings",
       "MultiProcessorCompile"
   }

 --  pchheader "pch.h"
 --  pchsource "pch/pch.cpp"

 targetdir (outputdir .. "%{prj.name}/")
 objdir    (objectdir .. "%{prj.name}/")

   files { "**.h", "**.hpp", "**.cpp" }

      -- Specify library directories based on the build configuration and StaticLib's location
   
      -- Link against the StaticLib project
      links { "BehemothCore", "freeglut" }

      filter "architecture:Win32"
      libdirs {
          "%{wks.location}/Source/NextAPI/glut/lib/", outputdir .. "BehemothCore/"
      }

  filter "architecture:x64"
      libdirs {
          "%{wks.location}/Source/NextAPI/glut/lib/x64/", outputdir .. "BehemothCore/"
      }

      includedirs { "%{wks.location}/Source/Engine/BehemothCore", "%{wks.location}/Source/NextAPI/" }
   
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