project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Elven/vendor/spdlog/include",
        "%{wks.location}/Elven/src",
        "%{wks.location}/Elven/vendor",
        "%{IncludeDir.glm}"
    }
    
    links 
    {
        "Elven"
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