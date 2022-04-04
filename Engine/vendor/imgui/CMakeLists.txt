set(IMGUI_SOURCES
imgui_demo.cpp
imgui_draw.cpp
imgui_widgets.cpp
imgui.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
        )