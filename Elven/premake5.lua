project "Elven"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "elpch.h"
    pchsource "src/elpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/glm/glm/**.hpp",
        "vendor/glm/glm/**.inl",
        "vendor/gdm/gdm/**.h"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    includedirs 
    {
        "src",
        "vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.gdm}",
        "%{wks.location}/Allocators/src"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "Allocators"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "EL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "EL_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EL_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "EL_DIST"
        runtime "Release"
        optimize "on"

include "Allocators"