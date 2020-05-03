workspace "Elven"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

startproject "Sandbox"

project "Elven"
    location "Elven"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "elpch.h"
    pchsource "Elven/src/elpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "EL_PLATFORM_WINDOWS",
            "EL_BUILD_DLL",
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "EL_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "EL_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "EL_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Elven/vendor/spdlog/include",
        "Elven/src"
    }
    
    links 
    {
        "Elven"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "EL_PLATFORM_WINDOWS"
        }

        filter "configurations:Debug"
            defines "EL_DEBUG"
            symbols "On"

        filter "configurations:Release"
            defines "EL_RELEASE"
            optimize "On"

        filter "configurations:Dist"
            defines "EL_DIST"
            optimize "On"