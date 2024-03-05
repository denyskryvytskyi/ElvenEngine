cmake_minimum_required(VERSION 3.10)

project(ElvenEngine
        VERSION 1.0
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CONFIGURATION_TYPES Debug Release)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(BUILD_SANDBOX "build sandbox projects" ON)
option(BUILD_GAMES "build games" ON)
option(EDITOR_MODE "Enable ImGUI editor overlay" ON)
option(PROFILE_MODE "Enable functions profiling" ON)
option(MODULE_3D_ENABLED "Enable 3D functionality with ASSIMP library" ON)

if(WIN32)
    add_compile_definitions(
        EL_PLATFORM_WINDOWS
        NOMINMAX
    )
else()
    message("This is not supported platform for now!")
endif()

if (EDITOR_MODE)
    add_compile_definitions(EDITOR_MODE)
endif()

if (PROFILE_MODE)
    add_compile_definitions(PROFILE_MODE)
endif()

add_subdirectory(Engine)

set(ElvenEngine_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if (BUILD_SANDBOX)
    add_subdirectory(Sandbox2D)
    add_subdirectory(Sandbox3D)

    if(CMAKE_GENERATOR MATCHES "Visual Studio")
        set_directory_properties(
            PROPERTIES
            VS_STARTUP_PROJECT "Sandbox2D"
        )
    endif()
endif()

if (BUILD_GAMES)
    add_subdirectory(Games)
endif()