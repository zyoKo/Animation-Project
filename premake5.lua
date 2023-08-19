workspace "3DAnimation"
    architecture "x86_64"
    startproject "3DAnimation"

    configurations
    {
        "Debug",
        "Release"
    }

    sourcedir = "./src"
    vendordir = "./vendor"
    outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

    group "3DAnimation"
        include "3DAnimation"
    group ""

newaction
{
    trigger = "clean",
    description = "Remove all binaries, intermediate binaries, and VS Files.",
    execute = function()
        print("Removing binaries")
        os.rmdir("./bin")
        print("Removing intermediate binaries")
        os.rmdir("./bin-int")
        print("Removing project files")
        os.rmdir("./.vs")
        os.remove("**.sln")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxproj.user")
        print("Done")
    end
}