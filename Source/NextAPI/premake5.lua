project "NextAPI"
   kind "StaticLib"
   language "C++"
    cppdialect    "C++14"
    staticruntime "off"
    floatingpoint "fast"

    targetdir (outputdir .. "%{prj.name}/")
    objdir    (objectdir .. "%{prj.name}/")

   files { "**.h", "**.hpp", "**.cpp" }

      -- Specify library directories based on the build configuration and StaticLib's location
      libdirs ("%{wks.location}/Source/NextAPI/glut/lib/x64")
   
      -- Link against the StaticLib project
      links { "freeglut", "dxguid", "dsound"}

      includedirs {"%{prj.location}/", ".", "./glut", "%{wks.location}/Source/NextAPI" }

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
