project "Sandbox"
    kind "ConsoleApp"
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

    defines
    {
        "GLFW_INCLUDE_NONE"
    }

    includedirs 
    {
        sourcedir,
        "../3DAnimation/src",
        "../3DAnimation/vendor",
        "../3DAnimation/vendor/glad/include",
        "../3DAnimation/vendor/GLFW/include"
    }

    links
    {
        "3DAnimation"
    }

    filter "system:windows"
        systemversion "latest"

    filter { "configurations:Debug" }
        buildoptions "/MTd"
        defines "ANIMATION_DEBUG"
        runtime "Debug"
        symbols "on"

    filter { "configurations:Release" }
        buildoptions "/MT"
        defines "ANIMATION_RELEASE"
        runtime "Release"
        optimize "on"