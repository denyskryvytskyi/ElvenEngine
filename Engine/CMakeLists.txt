set(ENGINE_HEADERS
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
"src/Renderer/TextRenderer.h"
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
"src/Scene/Systems/Render2dSystem.h"
"src/Scene/Systems/BehaviorSystem.h"
)

#editor stuff
if (EDITOR_MODE)
    set(ENGINE_HEADERS
    ${ENGINE_HEADERS}
    "src/ImGui/ImGuiOverlay.h"
    )
endif()

# src
set(ENGINE_SOURCES
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
"src/Renderer/TextRenderer.cpp"
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
"src/Scene/Systems/Render2dSystem.cpp"
"src/Scene/Systems/BehaviorSystem.cpp"
${ENGINE_HEADERS}
)

if (EDITOR_MODE)
    set(ENGINE_SOURCES
    ${ENGINE_SOURCES}
    "src/ImGui/ImGuiOverlay.cpp"
    )
endif()

# create static library
set(LIBRARY_NAME ElvenEngine)
add_library(${LIBRARY_NAME} STATIC ${ENGINE_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SOURCES})

# path to dependencies
set(VENDOR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

# third-party libs paths
set(glfw ${VENDOR_PATH}/GLFW)
set(glad ${VENDOR_PATH}/GLAD)
set(spdlog ${VENDOR_PATH}/spdlog)
set(lia ${VENDOR_PATH}/lia)
set(stb_image ${VENDOR_PATH}/stb_image)
set(json ${VENDOR_PATH}/json)
set(freetype ${VENDOR_PATH}/freetype)
if (EDITOR_MODE)
    set(imgui ${VENDOR_PATH}/imgui)
endif()

# include dirs
set(ENGINE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(ENGINE_INCLUDE_DIRS
${ENGINE_SOURCE_DIR}
${glad}/include
${glfw}/include
${lia}/lia
${spdlog}/include
${stb_image}/include
${json}/include
${freetype}/include
)

if (EDITOR_MODE)
    set(ENGINE_INCLUDE_DIRS
    ${ENGINE_INCLUDE_DIRS}
    ${imgui}/include
    )
endif()

target_include_directories(${LIBRARY_NAME}
    PUBLIC ${ENGINE_INCLUDE_DIRS}
)

# link dependencies
add_subdirectory(${VENDOR_PATH}/GLFW)
set_target_properties(glfw PROPERTIES FOLDER "Libs")
add_subdirectory(${VENDOR_PATH}/GLAD)
set_target_properties(glad PROPERTIES FOLDER "Libs")
add_subdirectory(${VENDOR_PATH}/spdlog)
set_target_properties(spdlog PROPERTIES FOLDER "Libs")
add_subdirectory(${VENDOR_PATH}/lia)
set_target_properties(lia PROPERTIES FOLDER "Libs")
add_subdirectory(${VENDOR_PATH}/stb)
set_target_properties(stb PROPERTIES FOLDER "Libs")
if (EDITOR_MODE)
    add_subdirectory(${VENDOR_PATH}/imgui)
    set_target_properties(imgui PROPERTIES FOLDER "Libs")
endif()

set(STATIC_THIRD_PARTY_LIBRARIES
glfw
glad
lia
spdlog
stb
)

if (EDITOR_MODE)
    set(STATIC_THIRD_PARTY_LIBRARIES
    ${STATIC_THIRD_PARTY_LIBRARIES}
    imgui
    )
endif()

target_link_libraries(${LIBRARY_NAME} ${STATIC_THIRD_PARTY_LIBRARIES}
        debug  ${freetype}/lib/freetyped.lib
        optimized ${freetype}/lib/freetype.lib)

# pch header file path
set(ENGINE_PRECOMPILED_HEADERS
    ${ENGINE_SOURCE_DIR}/elpch.h
)
target_precompile_headers(${LIBRARY_NAME} PRIVATE ${ENGINE_PRECOMPILED_HEADERS})

# compile definitions
add_compile_definitions(
    _CRT_SECURE_NO_WARNINGS
    GLFW_INCLUDE_NONE
)

target_compile_definitions(${LIBRARY_NAME} PRIVATE
  $<$<CONFIG:Debug>:DEBUG_MODE>
  $<$<CONFIG:Release>:RELEASE_MODE>
)