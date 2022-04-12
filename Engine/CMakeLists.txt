set(ENGINE_HEADERS
"src/elpch.h"
"src/Elven.h"
"src/Core/Application.h"
"src/Core/Core.h"
"src/Core/EntryPoint.h"
"src/Core/Input.h"
"src/Core/KeyCodes.h"
"src/Core/Layer.h"
"src/Core/LayerStack.h"
"src/Core/Log.h"
"src/Core/MouseCodes.h"
"src/Core/Semaphore.h"
"src/Core/Timer.h"
"src/Core/Window.h"
"src/Events/ApplicationEvent.h"
"src/Events/Event.h"
"src/Events/EventDispatcher.h"
"src/Events/EventManager.h"
"src/Events/KeyEvent.h"
"src/Events/MouseEvent.h"
"src/ImGui/imgui_impl_glfw.h"
"src/ImGui/imgui_impl_opengl3.h"
"src/ImGui/ImGuiLayer.h"
"src/Renderer/Buffer.h"
"src/Renderer/GraphicsContext.h"
"src/Renderer/OrthographicCamera.h"
"src/Renderer/OrthographicCameraController.h"
"src/Renderer/RenderCommand.h"
"src/Renderer/Renderer.h"
"src/Renderer/RendererAPI.h"
"src/Renderer/Shader.h"
"src/Renderer/VertexArray.h"
"src/Platform/Windows/WindowsWindow.h"
"src/Platform/OpenGL/OpenGLBuffer.h"
"src/Platform/OpenGL/OpenGLContext.h"
"src/Platform/OpenGL/OpenGLRendererAPI.h"
"src/Platform/OpenGL/OpenGLShader.h"
"src/Platform/OpenGL/OpenGLVertexArray.h"
)

set(ENGINE_SOURCES 
"src/Core/Application.cpp"
"src/Core/Layer.cpp"
"src/Core/LayerStack.cpp"
"src/Core/Log.cpp"
"src/Core/Window.cpp"
"src/Events/EventManager.cpp"
"src/Renderer/Buffer.cpp"
"src/Renderer/GraphicsContext.cpp"
"src/Renderer/OrthographicCamera.cpp"
"src/Renderer/OrthographicCameraController.cpp"
"src/Renderer/RenderCommand.cpp"
"src/Renderer/Renderer.cpp"
"src/Renderer/RendererAPI.cpp"
"src/Renderer/Shader.cpp"
"src/Renderer/VertexArray.cpp"
"src/ImGui/imgui_impl_glfw.cpp"
"src/ImGui/imgui_impl_opengl3.cpp"
"src/ImGui/ImGuiLayer.cpp"
"src/Platform/OpenGL/OpenGLBuffer.cpp"
"src/Platform/OpenGL/OpenGLContext.cpp"
"src/Platform/OpenGL/OpenGLRendererAPI.cpp"
"src/Platform/OpenGL/OpenGLShader.cpp"
"src/Platform/OpenGL/OpenGLVertexArray.cpp"
"src/Platform/Windows/WindowsInput.cpp"
"src/Platform/Windows/WindowsWindow.cpp"
${ENGINE_HEADERS}
)

set(LIBRARY_NAME ElvenEngine)

# path to dependencies
set(VENDOR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

# create static library
add_library(${LIBRARY_NAME} STATIC ${ENGINE_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SOURCES})

# 3rd party libs list
set(glfw ${VENDOR_PATH}/GLFW)
set(glad ${VENDOR_PATH}/GLAD)
set(spdlog ${VENDOR_PATH}/spdlog)
set(glm ${VENDOR_PATH}/glm)
set(imgui ${VENDOR_PATH}/imgui)
set(lia ${VENDOR_PATH}/lia)

set(STATIC_THIRD_PARTY_LIBRARIES
    glfw
    glad
    spdlog
    glm
    imgui
    lia
)

add_subdirectory(${glfw})
add_subdirectory(${glad})
add_subdirectory(${spdlog})
add_subdirectory(${glm})
add_subdirectory(${imgui})
add_subdirectory(${lia})

# link dependencies
target_link_libraries(${LIBRARY_NAME} ${STATIC_THIRD_PARTY_LIBRARIES})

set(ENGINE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(ENGINE_INCLUDE_DIRS
    ${ENGINE_SOURCE_DIR}
    ${glfw}/include
    ${glad}/include
    ${spdlog}/include
    ${glm}/glm
    ${imgui}
    )

# state that anybody linking to us needs to include the current source dir, while we don't
target_include_directories(ElvenEngine
        PUBLIC ${ENGINE_INCLUDE_DIRS}
        )

# pch header file path
set(ENGINE_PRECOMPILED_HEADERS
    ${ENGINE_SOURCE_DIR}/elpch.h
)
target_precompile_headers(ElvenEngine PRIVATE ${ENGINE_PRECOMPILED_HEADERS})

add_compile_definitions(
    _CRT_SECURE_NO_WARNINGS
    GLFW_INCLUDE_NONE
)