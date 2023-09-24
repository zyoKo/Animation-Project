project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("./build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("./build/obj/" .. outputdir .. "/%{prj.name}")

    linkoptions 
    {
        "/NODEFAULTLIB:LIBCMTD.lib"
    }

    files 
    {
        sourcedir .. "/**.cpp",
        sourcedir .. "/**.h"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs 
    {
        sourcedir,
        "../Animation-Engine/src",
        "../Animation-Engine/vendor",
        "../Animation-Engine/vendor/glad/include",
        "../Animation-Engine/vendor/GLFW/include",
        "../Animation-Engine/vendor/spdlog/include"
    }

    links
    {
        "Animation-Engine"
    }

    filter "system:windows"
        systemversion "latest"

    filter { "configurations:Debug" }
        buildoptions "/MTd"
        runtime "Debug"
        symbols "on"

    filter { "configurations:Release" }
        buildoptions "/MT"
        runtime "Release"
        optimize "on"