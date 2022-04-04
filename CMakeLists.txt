cmake_minimum_required(VERSION 3.10)

project(ElvenEngine
        VERSION 1.0
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

option(ELVENENGINE_BUILD_RELEASE "release build for end user" OFF)
option(ELVENENGINE_BUILD_SANDBOX "build sandbox project" ON)

if(WIN32)
    add_compile_definitions(
        EL_PLATFORM_WINDOWS
        NOMINMAX
    )
else()
    message("This is not supported platform for now!")
endif()

if(ELVENENGINE_BUILD_RELEASE)
    set(CMAKE_BUILD_TYPE "Release")
    add_compile_definitions(MXENGINE_SHIPPING)
endif()
add_compile_definitions(MXENGINE_CMAKE_BUILD)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

add_subdirectory(Engine)

set(ElvenEngine_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if (ELVENENGINE_BUILD_SANDBOX)
    add_subdirectory(Sandbox)
    if(CMAKE_GENERATOR MATCHES "Visual Studio")
        set_directory_properties(
            PROPERTIES
            VS_STARTUP_PROJECT "Sandbox"
        )
    endif()
endif()