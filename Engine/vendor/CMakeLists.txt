cmake_minimum_required(VERSION 3.10)

project(vendor
VERSION 1.0
LANGUAGES CXX C)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(lia)
add_subdirectory(GLAD)
add_subdirectory(GLFW)
add_subdirectory(spdlog)
add_subdirectory(stb)
add_subdirectory(freetype)
add_subdirectory(fmt)
add_subdirectory(imgui)
# reset flag to build assimp dll
set(BUILD_SHARED_LIBS ON) 
add_subdirectory(assimp)