set(SANDBOX_SOURCES
    "src/ExampleLayer.h"
    "src/ExampleLayer.cpp"
    "src/SandboxApp.h"
    "src/SandboxApp.cpp"
    "src/TestAllocatorsLayer.h"
    "src/TestAllocatorsLayer.cpp"
    "res/shaders/shader.frag"
    "res/shaders/shader.vert"
)

set(PROJECT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ENGINE_INCLUDE_DIR}
)

add_executable(Sandbox ${SANDBOX_SOURCES})

target_link_libraries(Sandbox PUBLIC ElvenEngine)

target_include_directories(ElvenEngine
        PUBLIC ${PROJECT_INCLUDE_DIRECTORIES}
        )