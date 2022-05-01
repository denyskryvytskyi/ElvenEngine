set(SANDBOX_SOURCES
    "src/ExampleLayer.h"
    "src/ExampleLayer.cpp"
    "src/SandboxApp.h"
    "src/SandboxApp.cpp"
    "src/TestAllocatorsLayer.h"
    "src/TestAllocatorsLayer.cpp"
    "assets/shaders/shader.frag"
    "assets/shaders/shader.vert"
    "assets/shaders/texture_shader.frag"
    "assets/shaders/texture_shader.vert"
)

set(PROJECT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(Sandbox ${SANDBOX_SOURCES})

target_link_libraries(Sandbox PUBLIC ElvenEngine)

target_include_directories(Sandbox
        PRIVATE ${PROJECT_INCLUDE_DIRECTORIES}
        )

set_target_properties(Sandbox PROPERTIES 
                      VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)