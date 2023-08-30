project "Animation-Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("./build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("./build/obj/" .. outputdir .. "/%{prj.name}")

    files 
    {
        sourcedir .. "/**.cpp",
        sourcedir .. "/**.h"
    }

    includedirs 
    {
        sourcedir,
        vendordir .. "/GLFW/include",
        vendordir .. "/glad/include",
        vendordir .. "/spdlog/include"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "OPEN_GL",
        "_CRT_SECURE_NO_WARNINGS"
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib"
    }

    filter { "configurations:Debug" }
        buildoptions "/MTd"
        defines "ANIM_DEBUG"
        runtime "Debug"
        symbols "on"

    filter { "configurations:Release" }
        buildoptions "/MT"
        runtime "Release"
        optimize "on"