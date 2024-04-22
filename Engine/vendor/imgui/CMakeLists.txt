set(IMGUI_SOURCES
"src/imgui_demo.cpp"
"src/imgui_draw.cpp"
"src/imgui_widgets.cpp"
"src/imgui.cpp"
"src/imgui_impl_glfw.cpp"
"src/imgui_impl_opengl3.cpp"
)

add_library(imgui STATIC ${IMGUI_SOURCES})

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/GLFW/include
    ${CMAKE_SOURCE_DIR}/GLAD/include
)

target_include_directories(imgui PUBLIC ${INCLUDE_DIRS})