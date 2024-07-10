cmake_minimum_required(VERSION 3.11)

project(ElvenEngine
        VERSION 1.0
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CONFIGURATION_TYPES Debug Release)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

option(BUILD_SANDBOX "build sandbox projects" ON)
option(BUILD_GAMES "build games" ON)
option(PROFILE_MODE "Enable functions profiling" ON)
option(MODULE_EDITOR_ENABLED "Enable ImGUI-based editor" ON)
option(MODULE_3D_ENABLED "Enable 3D module with Assimp support" ON)
option(MODULE_SOUND_ENABLED "Enable sound module with irrklang support" ON)

if(WIN32)
    add_compile_definitions(
        EL_PLATFORM_WINDOWS
        NOMINMAX
    )
else()
    if (APPLE)
        # something
    else()
        message("This is not supported platform for now!")
    endif()
endif()

if (PROFILE_MODE)
    add_compile_definitions(PROFILE_MODE)
endif()

if (MODULE_EDITOR_ENABLED)
    add_compile_definitions(MODULE_EDITOR_ENABLED)
endif()

if (MODULE_3D_ENABLED)
    add_compile_definitions(MODULE_3D_ENABLED)
endif()

if (MODULE_SOUND_ENABLED)
    add_compile_definitions(MODULE_SOUND_ENABLED)
endif()

add_subdirectory(Engine)

set(ElvenEngine_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if (BUILD_SANDBOX)
    if (MODULE_3D_ENABLED)
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