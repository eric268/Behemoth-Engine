project "Engine"
kind "StaticLib"
language "C++"
 cppdialect    "C++20"
 staticruntime "off"
 floatingpoint "fast"

   targetdir (outputdir .. "%{prj.name}/")
   objdir    (objectdir .. "%{prj.name}/")
   
   files { "**.h", "**.hpp", "**.cpp" }

      -- Specify library directories based on the build configuration and StaticLib's location
   
      -- Link against the StaticLib project
      links { "NextAPI", "freeglut" }

      defines
      {
          "WORLD_SCALE=100"
      }

      includedirs { "%{wks.location}/Source/Engine/BehemothCore", "%{wks.location}/Source/NextAPI/App/" }
     filter "architecture:Win32"
         libdirs 
         {
             "%{wks.location}/Source/NextAPI/glut/lib/", outputdir .. "NextAPI/"
         }
 
     filter "architecture:x64"
         libdirs 
         {
             "%{wks.location}/Source/NextAPI/glut/lib/x64/", outputdir .. "NextAPI/"
         }
 
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