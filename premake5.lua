workspace "BehemothEngine"

     if not _OPTIONS["prj"] and not _OPTIONS["new"] then
          _OPTIONS["prj"] = "NextGame"
     end
   -- os.execute("rmdir /s /q .vs")
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }

   startproject "NextEntry"
   outputdir = "%{wks.location}/Build/bin/%{cfg.buildcfg}/%{cfg.system}/"
   objectdir = "%{wks.location}/Build/obj/%{cfg.buildcfg}/%{cfg.system}/"

   newoption
   {
        trigger = "prj",
        value = "NAME",
        description = "Specify the name of the game you wish to create"
   }

   newoption
   {
        trigger = "new",
        description = "Set to true to generate a new project"
   }

   group "Engine"

        include "Source/Engine"
        
            include "Source/EntryPoint"

    group "Tests"

        include "Source/Tests/BehemothTests"

    group "Vendor"

        include "Source/NextAPI"

     if _OPTIONS["prj"] then
     group "Game"
          include("Source/Games/" .. _OPTIONS["prj"])
          
     else
          print("Error - Project not found")
     end

    