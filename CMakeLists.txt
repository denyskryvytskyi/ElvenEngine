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
option(ASSIMP_MODE "Enable 3D module with Assimp support" ON)

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

if (ASSIMP_MODE)
    add_compile_definitions(ASSIMP_MODE)
endif()

add_subdirectory(Engine)

set(ElvenEngine_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})


if (BUILD_SANDBOX)
    if (ASSIMP_MODE)
        add_subdirectory(Sandbox3D)
        set(STARTUP_PROJECT_NAME "Sandbox3D")
    else()
        add_subdirectory(Sandbox2D)
        set(STARTUP_PROJECT_NAME "Sandbox2D")
    endif()

    if(CMAKE_GENERATOR MATCHES "Visual Studio")
        set_directory_properties(
            PROPERTIES
            VS_STARTUP_PROJECT ${STARTUP_PROJECT_NAME}
        )
    endif()
endif()

if (BUILD_GAMES)
    add_subdirectory(Games)
endif()