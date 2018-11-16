workspace "a.out"
        language "C++"
        architecture "x86_64"

        configurations { "Debug", "Release" }

        filter { "configurations:Debug" }
                symbols "On"

        filter { "configurations:Release" }
                optimize "On"

        filter { }

        targetdir "bin/%{cfg.longname}/"
        objdir "obj/%{cfg.longname}/"

project "client"
        kind "ConsoleApp"
        files { "client/**.hpp", "client/**.cpp" }

        includedirs "src/client"

project "server"
        kind "ConsoleApp"
        files { "server/**.hpp", "server/**.cpp" }

        includedirs "src/server"

newaction {
        trigger = "clean",
        description = "Clean the workspace",
        execute = function ()
                os.rmdir "./obj"
                os.rmdir "./bin"
                os.remove "*.make"
                os.remove "Makefile"
                print "Finished."
        end
}
