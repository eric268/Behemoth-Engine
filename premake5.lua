workspace "BehemothEngine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }

   startproject "NextEntry"
   outputdir = "%{wks.location}/Build/bin/%{cfg.buildcfg}/%{cfg.system}/"
   objectdir = "%{wks.location}/Build/obj/%{cfg.buildcfg}/%{cfg.system}/"

   group "Engine"

        include "Source/Engine"

        group "Engine"

            include "Source/EntryPoint"

    group "Sandbox"
        
        include "Source/Sandbox"

    group "Tests"

        include "Source/Tests/BehemothTests"

    group "Vendor"

        include "Source/NextAPI"
    