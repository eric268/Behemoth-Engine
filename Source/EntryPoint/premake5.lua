project "NextEntry"
   kind "WindowedApp"
   language "C++"
   cppdialect    "C++20"
    staticruntime "off"
    floatingpoint "fast"


   targetdir (outputdir .. "%{prj.name}/")
   objdir    (objectdir .. "%{prj.name}/")


   files { "%{wks.location}/Source/EntryPoint/**.h",
           "%{wks.location}/Source/EntryPoint/**.hpp",
           "%{wks.location}/Source/EntryPoint/**.cpp" }

      -- Specify library directories based on the build configuration and StaticLib's location

   
      -- Link against the StaticLib project
      links { "Engine", "NextAPI", _OPTIONS["prj"], "freeglut" }

      includedirs { "%{wks.location}/Source/Engine/BehemothCore", "%{wks.location}/Source/"}
 
    filter "architecture:Win32"
        libdirs {"%{wks.location}/Source/NextAPI/glut/lib/", 
        outputdir .. "Engine/",
        outputdir .. "NextAPI/",
        outputdir .. _OPTIONS["prj"]
        }

        glutLoc = "%{wks.location}/Source"
        outputLoc = outputdir .. "%{prj.name}/"
         postbuildcommands 
         {
            "{COPY} \"%{wks.location}/Source/NextAPI/glut/bin/*.dll\" \"" .. outputdir .. "%{prj.name}/\""
         }

    filter "architecture:x64"
        libdirs {"%{wks.location}/Source/NextAPI/glut/lib/x64/",
        outputdir .. "Engine/",
        outputdir .. "NextAPI/",
        outputdir .. _OPTIONS["prj"] 
        }

        postbuildcommands 
        {
         "{COPY} \"%{wks.location}/Source/NextAPI/glut/bin/x64/*.dll\" \"" .. outputdir .. "%{prj.name}/\""
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