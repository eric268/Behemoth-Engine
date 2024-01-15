project "BehemothTests"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "off"
   floatingpoint "fast"
   rtti "on"

   nuget { "Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn:1.8.1.7" }
   
   flags { "FatalWarnings", "MultiProcessorCompile" }

   pchheader "pch.h"
   pchsource "pch.cpp"

   targetdir (outputdir .. "%{prj.name}/")
   objdir (outputdir .. "%{prj.name}/obj/") -- Changed to a more typical obj directory

   files { "%{wks.location}/Source/Tests/BehemothTests/**.h",
           "%{wks.location}/Source/Tests/BehemothTests/**.hpp",
           "%{wks.location}/Source/Tests/BehemothTests/**.cpp" }

   links {"Engine","NextAPI", "freeglut" }

   filter "architecture:Win32"
      libdirs { "%{wks.location}/Source/NextAPI/glut/lib/", outputdir .. "Engine/",
                "%{wks.location}/Source/NextAPI/glut/lib/", outputdir .. "NextAPI/" }

   filter "architecture:x64"
      libdirs { "%{wks.location}/Source/NextAPI/glut/lib/x64/", outputdir .. "Engine/",
               "%{wks.location}/Source/NextAPI/glut/lib/", outputdir .. "NextAPI/" }

   includedirs { "%{wks.location}/Source/Engine/BehemothCore", "%{wks.location}/Source/", "%{wks.location}/Source/NextAPI/" }

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
