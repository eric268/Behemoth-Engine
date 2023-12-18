workspace "BehemothEngine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }

   startproject "NextEntry"
   outputdir = "%{wks.location}/Build/bin/%{cfg.buildcfg}/%{cfg.system}/"
   objectdir = "%{wks.location}/Build/obj/%{cfg.buildcfg}/%{cfg.system}/"

   newoption
   {
        trigger = "prjname",
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

     if not _OPTIONS["prjname"] then
          _OPTIONS["prjname"] = "Sandbox"
     end

     if _OPTIONS["prjname"] then
     group "Game"
          include("Source/Games/" .. _OPTIONS["prjname"])
          
     else
          print("Error - Project not found")
     end

    