project "Animation-Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("./build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("./build/obj/" .. outputdir .. "/%{prj.name}")

    pchheader "AnimationPch.h"
    pchsource "src/AnimationPch.cpp"

    files 
    {
        sourcedir .. "/**.cpp",
        sourcedir .. "/**.h",
        sourcedir .. "/**.inl"
    }

    includedirs 
    {
        sourcedir,
        vendordir .. "/GLFW/include",
        vendordir .. "/glad/include",
        vendordir .. "/spdlog/include",
        vendordir .. "/stb/include",
        vendordir .. "/glm/include",
        vendordir .. "/assimp/include"
    }

    libdirs
    {
        vendordir .. "/assimp/lib"
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
        "OPEN_GL",
        "_CRT_SECURE_NO_WARNINGS",
        "STB_IMAGE_IMPLEMENTATION"
    }

    links
    {
        "opengl32.lib",
        "GLFW",
        "Glad",
        "assimp-vc143-mt.lib"
    }

    filter "system:windows"
        systemversion "latest"

        prebuildcommands 
        {
            "copy \"$(ProjectDir)vendor\\assimp\\lib\\*.dll\" \"$(ProjectDir)$(OutDir)\""  
        }

    filter { "configurations:Debug" }
        buildoptions "/MTd"
        defines "ANIM_DEBUG"
        runtime "Debug"
        symbols "on"

    filter { "configurations:Release" }
        buildoptions "/MT"
        defines "ANIM_RELEASE"
        runtime "Release"
        optimize "on"