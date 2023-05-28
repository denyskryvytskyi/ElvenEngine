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
"src/Core/Profiler.h"
"src/Core/TimeHelpers.h"
"src/Events/ApplicationEvent.h"
"src/Events/Event.h"
"src/Events/EventHandler.h"
"src/Events/EventManager.h"
"src/Events/KeyEvent.h"
"src/Events/MouseEvent.h"
"src/Events/TextureEvent.h"
"src/Events/PhysicsEvent.h"
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
"src/Scene/Systems/Physics2dSystem.h"
"src/Resources/TextureManager.h"
"src/Resources/AudioManager.h"
"src/Resources/FontManager.h"
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
"src/Scene/Systems/Physics2dSystem.cpp"
"src/Resources/TextureManager.cpp"
"src/Resources/AudioManager.cpp"
"src/Resources/FontManager.cpp"
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

# include dirs
set(ENGINE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(ENGINE_INCLUDE_DIRS
${ENGINE_SOURCE_DIR}
${VENDOR_PATH}/GLAD/include
${VENDOR_PATH}/GLFW/include
${VENDOR_PATH}/lia
${VENDOR_PATH}/spdlog/include
${VENDOR_PATH}/stb/include
${VENDOR_PATH}/json/include
${VENDOR_PATH}/freetype/include
${VENDOR_PATH}/irrklang/include
${VENDOR_PATH}/fmt/include
)

if (EDITOR_MODE)
    set(ENGINE_INCLUDE_DIRS
        ${ENGINE_INCLUDE_DIRS}
        ${VENDOR_PATH}/imgui/include
    )
endif()

target_include_directories(${LIBRARY_NAME}
    PUBLIC ${ENGINE_INCLUDE_DIRS}
)

if (EDITOR_MODE)
    add_subdirectory(${VENDOR_PATH}/imgui)
    set_target_properties(imgui PROPERTIES FOLDER "Libs")
endif()

if (EDITOR_MODE)
    set(STATIC_THIRD_PARTY_LIBRARIES
        imgui
    )
endif()

set(VENDOR_PATH_DEBUG ${PROJECT_SOURCE_DIR}/build_vendor/lib/Debug/)
set(VENDOR_PATH_RELEASE ${PROJECT_SOURCE_DIR}/build_vendor/lib/Release/)

find_library(glfw_d NAMES glfw3 PATHS ${VENDOR_PATH_DEBUG})
find_library(glad_d NAMES glad PATHS ${VENDOR_PATH_DEBUG})
find_library(spdlog_d NAMES spdlogd PATHS ${VENDOR_PATH_DEBUG})
find_library(lia_d NAMES lia PATHS ${VENDOR_PATH_DEBUG})
find_library(stb_d NAMES stb PATHS ${VENDOR_PATH_DEBUG})
find_library(freetype_d NAMES freetyped PATHS ${VENDOR_PATH_DEBUG})
find_library(fmt_d NAMES fmtd PATHS ${VENDOR_PATH_DEBUG})

find_library(glad NAMES glad PATHS ${VENDOR_PATH_RELEASE})
find_library(glfw NAMES glfw3 PATHS ${VENDOR_PATH_RELEASE})
find_library(spdlog NAMES spdlog PATHS ${VENDOR_PATH_RELEASE})
find_library(lia NAMES lia PATHS ${VENDOR_PATH_RELEASE})
find_library(stb NAMES stb PATHS ${VENDOR_PATH_RELEASE})
find_library(freetype NAMES freetype PATHS ${VENDOR_PATH_RELEASE})
find_library(fmt NAMES fmt PATHS ${VENDOR_PATH_RELEASE})
find_library(irrklang NAMES irrKlang.lib PATHS ${VENDOR_PATH}/irrklang/lib)

set(VENDOR
debug ${glfw_d} optimized ${glfw}
debug ${glad_d} optimized ${glad}
debug ${spdlog_d} optimized ${spdlog}
debug ${lia_d} optimized ${lia}
debug ${stb_d} optimized ${stb}
debug ${freetype_d} optimized ${freetype}
debug ${irrklang} optimized ${irrklang}
debug ${fmt_d} optimized ${fmt}
)

target_link_libraries(${LIBRARY_NAME} ${STATIC_THIRD_PARTY_LIBRARIES} ${VENDOR})

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