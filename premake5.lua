workspace "a.out"
        language "C++"
        architecture "x86_64"

        targetdir "bin/%{cfg.longname}/"
        objdir "obj/%{cfg.longname}/"

        configurations { "Debug", "Release" }

        filter { "system:windows" }
                defines "AOUT_PLATFORM_WINDOWS"

        filter { "system:linux" }
                defines "AOUT_PLATFORM_LINUX"

        filter { "configurations:Debug" }
                symbols "On"

        filter { "configurations:Release" }
                optimize "On"

        filter { }

project "common"
        kind "SharedLib"
        files { "common/**.hpp", "common/**.cpp" }
        defines "AOUT_EXPORTS"

project "client"
        kind "ConsoleApp"
        files { "client/**.hpp", "client/**.cpp" }
        includedirs { "." }
        links { "common" }

project "server"
        kind "ConsoleApp"
        files { "server/**.hpp", "server/**.cpp" }
        includedirs { "." }
        links { "common" }

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
