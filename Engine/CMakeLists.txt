set(ENGINE_HEADERS
"vendor/stb_image/stb_image.h"
"vendor/json/json.hpp"
"vendor/json/json_fwd.hpp"
"src/elpch.h"
"src/Elven.h"
"src/Core/Application.h"
"src/Core/Core.h"
"src/Core/EntryPoint.h"
"src/Core/Input.h"
"src/Core/KeyCodes.h"
"src/Core/Log.h"
"src/Core/MouseCodes.h"
"src/Core/Semaphore.h"
"src/Core/Timer.h"
"src/Core/Window.h"
"src/Core/FileSystem.h"
"src/Core/StringId.h"
"src/Core/CRC32.h"
"src/Core/SettingsConfig.h"
"src/Events/ApplicationEvent.h"
"src/Events/Event.h"
"src/Events/EventHandler.h"
"src/Events/EventManager.h"
"src/Events/KeyEvent.h"
"src/Events/MouseEvent.h"
"src/Events/TextureEvent.h"
"src/ImGui/ImGuiOverlay.h"
"src/Renderer/Buffer.h"
"src/Renderer/GraphicsContext.h"
"src/Renderer/Camera.h"
"src/Renderer/CameraController.h"
"src/Renderer/OrthographicCameraController.h"
"src/Renderer/RenderCommand.h"
"src/Renderer/Renderer2D.h"
"src/Renderer/Renderer.h"
"src/Renderer/RendererAPI.h"
"src/Renderer/Shader.h"
"src/Renderer/VertexArray.h"
"src/Renderer/Texture2D.h"
"src/Renderer/TextureManager.h"
"src/Renderer/Helpers2d.h"
"src/Platform/Windows/WindowsWindow.h"
"src/Platform/OpenGL/OpenGLBuffer.h"
"src/Platform/OpenGL/OpenGLContext.h"
"src/Platform/OpenGL/OpenGLRendererAPI.h"
"src/Platform/OpenGL/OpenGLShader.h"
"src/Platform/OpenGL/OpenGLVertexArray.h"
"src/Platform/OpenGL/OpenGLTexture2D.h"
"src/Scene/Entity.h"
"src/Scene/Component.h"
"src/Scene/Behavior.h"
"src/Scene/Scene.h"
"src/Scene/ComponentSystem.h"
"src/Scene/SceneManager.h"
"src/Scene/SceneSerializer.h"
"src/Scene/Components/SceneComponents.h"
"src/Scene/Systems/SpriteRenderSystem.h"
"src/Scene/Systems/BehaviorSystem.h"
)

set(ENGINE_SOURCES
"vendor/stb_image/stb_image.cpp"
"vendor/imgui/imgui_impl_glfw.cpp"
"vendor/imgui/imgui_impl_opengl3.cpp"
"src/Core/Application.cpp"
"src/Core/EntryPoint.cpp"
"src/Core/Log.cpp"
"src/Core/Window.cpp"
"src/Core/SettingsConfig.cpp"
"src/Events/EventManager.cpp"
"src/Renderer/Buffer.cpp"
"src/Renderer/GraphicsContext.cpp"
"src/Renderer/Camera.cpp"
"src/Renderer/CameraController.cpp"
"src/Renderer/OrthographicCameraController.cpp"
"src/Renderer/RenderCommand.cpp"
"src/Renderer/Renderer2D.cpp"
"src/Renderer/Renderer.cpp"
"src/Renderer/RendererAPI.cpp"
"src/Renderer/Shader.cpp"
"src/Renderer/VertexArray.cpp"
"src/Renderer/TextureManager.cpp"
"src/Renderer/Helpers2d.cpp"
"src/ImGui/ImGuiOverlay.cpp"
"src/Platform/OpenGL/OpenGLBuffer.cpp"
"src/Platform/OpenGL/OpenGLContext.cpp"
"src/Platform/OpenGL/OpenGLRendererAPI.cpp"
"src/Platform/OpenGL/OpenGLShader.cpp"
"src/Platform/OpenGL/OpenGLVertexArray.cpp"
"src/Platform/OpenGL/OpenGLTexture2D.cpp"
"src/Platform/Windows/WindowsInput.cpp"
"src/Platform/Windows/WindowsWindow.cpp"
"src/Scene/Behavior.cpp"
"src/Scene/Component.cpp"
"src/Scene/ComponentSystem.cpp"
"src/Scene/Scene.cpp"
"src/Scene/SceneManager.cpp"
"src/Scene/SceneSerializer.cpp"
"src/Scene/Components/SceneComponents.cpp"
"src/Scene/Systems/SpriteRenderSystem.cpp"
"src/Scene/Systems/BehaviorSystem.cpp"
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
set(imgui ${VENDOR_PATH}/imgui)
set(lia ${VENDOR_PATH}/lia)
set(stb_image ${VENDOR_PATH}/stb_image)
set(json ${VENDOR_PATH}/json)

set(STATIC_THIRD_PARTY_LIBRARIES
    glfw
    glad
    spdlog
    imgui
    lia
)

add_subdirectory(${glfw})
add_subdirectory(${glad})
add_subdirectory(${spdlog})
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
    ${imgui}
    ${stb_image}
    ${json}
    )

# state that anybody linking to us needs to include the current source dir, while we don't
target_include_directories(${LIBRARY_NAME}
        PUBLIC ${ENGINE_INCLUDE_DIRS}
        )

# pch header file path
set(ENGINE_PRECOMPILED_HEADERS
    ${ENGINE_SOURCE_DIR}/elpch.h
)
target_precompile_headers(${LIBRARY_NAME} PRIVATE ${ENGINE_PRECOMPILED_HEADERS})

add_compile_definitions(
    _CRT_SECURE_NO_WARNINGS
    GLFW_INCLUDE_NONE
)

target_compile_definitions(${LIBRARY_NAME} PRIVATE
  $<$<CONFIG:Debug>:DEBUG_MODE>
  $<$<CONFIG:Release>:RELEASE_MODE>
)