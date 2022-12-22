cmake_minimum_required(VERSION 3.10)

project(ElvenEngine
        VERSION 1.0
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

option(BUILD_SANDBOX "build sandbox project" ON)

if(WIN32)
    add_compile_definitions(
        EL_PLATFORM_WINDOWS
        NOMINMAX
    )
else()
    message("This is not supported platform for now!")
endif()

add_subdirectory(Engine)

set(ElvenEngine_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if (BUILD_SANDBOX)
    add_subdirectory(Sandbox)
    if(CMAKE_GENERATOR MATCHES "Visual Studio")
        set_directory_properties(
            PROPERTIES
            VS_STARTUP_PROJECT "Sandbox"
        )
    endif()
endif()