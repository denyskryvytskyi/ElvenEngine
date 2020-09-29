workspace "Elven"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Elven/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Elven/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Elven/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Elven/vendor/glm"

group "Dependencies"
    include "Elven/vendor/GLFW"
    include "Elven/vendor/Glad"
    include "Elven/vendor/imgui"
group ""

include "Elven"
include "Sandbox"