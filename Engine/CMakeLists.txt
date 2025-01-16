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
"src/Events/MeshEvent.h"
"src/Renderer/RHI/Buffer.h"
"src/Renderer/RHI/GraphicsContext.h"
"src/Renderer/RHI/RendererAPI.h"
"src/Renderer/RHI/Shader.h"
"src/Renderer/RHI/VertexArray.h"
"src/Renderer/RHI/Texture.h"
"src/Renderer/RHI/RenderTarget.h"
"src/Renderer/RHI/Cubemap.h"
"src/Renderer/Camera.h"
"src/Renderer/CameraController.h"
"src/Renderer/OrthographicCameraController.h"
"src/Renderer/EditorCameraController.h"
"src/Renderer/Renderer2D.h"
"src/Renderer/Renderer.h"
"src/Renderer/TextRenderer.h"
"src/Renderer/Material.h"
"src/Renderer/Mesh.h"
"src/Renderer/RenderTopology.h"
"src/Renderer/PostProcessor.h"
"src/Renderer/Skybox.h"
"src/Platform/Windows/WindowsWindow.h"
"src/Platform/OpenGL/OpenGLBuffer.h"
"src/Platform/OpenGL/OpenGLContext.h"
"src/Platform/OpenGL/OpenGLRendererAPI.h"
"src/Platform/OpenGL/OpenGLShader.h"
"src/Platform/OpenGL/OpenGLVertexArray.h"
"src/Platform/OpenGL/OpenGLTexture.h"
"src/Platform/OpenGL/OpenGLRenderTarget.h"
"src/Platform/OpenGL/OpenGLCubemap.h"
"src/Scene/Entity.h"
"src/Scene/Component.h"
"src/Scene/Behavior.h"
"src/Scene/Scene.h"
"src/Scene/ComponentSystem.h"
"src/Scene/SceneManager.h"
"src/Scene/SceneSerializer.h"
"src/Scene/SceneGraph.h"
"src/Scene/Components/ComponentSerializerHelper.h"
"src/Scene/Components/SceneComponents.h"
"src/Scene/Components/TransformComponent.h"
"src/Scene/Components/StaticMeshComponent.h"
"src/Scene/Components/LightComponent.h"
"src/Scene/Systems/Render2dSystem.h"
"src/Scene/Systems/BehaviorSystem.h"
"src/Scene/Systems/Physics2dSystem.h"
"src/Scene/Systems/RenderSystem.h"
"src/Scene/Systems/LightSystem.h"
"src/Resources/TextureManager.h"
"src/Resources/AudioManager.h"
"src/Resources/FontManager.h"
"src/Resources/MeshLibrary.h"
"src/Resources/ModelImporter.h"
)

#editor stuff
if (MODULE_EDITOR_ENABLED)
    set(ENGINE_HEADERS
    ${ENGINE_HEADERS}
    "src/Editor/ImGuiOverlay.h"
    "src/Editor/EditorHelpers.h"
    "src/Editor/Editor.h"
    "src/Editor/Panels/SceneHierarchyPanel.h"
    "src/Editor/Panels/ProfileTelemetryPanel.h"
    )
endif()

# src
set(ENGINE_SOURCES
"src/Core/Application.cpp"
"src/Core/EntryPoint.cpp"
"src/Core/Log.cpp"
"src/Core/Window.cpp"
"src/Core/SettingsConfig.cpp"
"src/Core/Profiler.cpp"
"src/Events/EventManager.cpp"
"src/Renderer/RHI/Buffer.cpp"
"src/Renderer/RHI/GraphicsContext.cpp"
"src/Renderer/RHI/RendererAPI.cpp"
"src/Renderer/RHI/Shader.cpp"
"src/Renderer/RHI/VertexArray.cpp"
"src/Renderer/RHI/RenderTarget.cpp"
"src/Renderer/Camera.cpp"
"src/Renderer/CameraController.cpp"
"src/Renderer/OrthographicCameraController.cpp"
"src/Renderer/EditorCameraController.cpp"
"src/Renderer/Renderer2D.cpp"
"src/Renderer/Renderer.cpp"
"src/Renderer/TextRenderer.cpp"
"src/Renderer/Mesh.cpp"
"src/Renderer/Material.cpp"
"src/Renderer/PostProcessor.cpp"
"src/Renderer/Skybox.cpp"
"src/Platform/OpenGL/OpenGLBuffer.cpp"
"src/Platform/OpenGL/OpenGLContext.cpp"
"src/Platform/OpenGL/OpenGLRendererAPI.cpp"
"src/Platform/OpenGL/OpenGLShader.cpp"
"src/Platform/OpenGL/OpenGLVertexArray.cpp"
"src/Platform/OpenGL/OpenGLTexture.cpp"
"src/Platform/OpenGL/OpenGLRenderTarget.cpp"
"src/Platform/OpenGL/OpenGLCubemap.cpp"
"src/Platform/Windows/WindowsInput.cpp"
"src/Platform/Windows/WindowsWindow.cpp"
"src/Scene/Behavior.cpp"
"src/Scene/Component.cpp"
"src/Scene/ComponentSystem.cpp"
"src/Scene/Scene.cpp"
"src/Scene/SceneManager.cpp"
"src/Scene/SceneSerializer.cpp"
"src/Scene/SceneGraph.cpp"
"src/Scene/Components/ComponentSerializerHelper.cpp"
"src/Scene/Components/SceneComponents.cpp"
"src/Scene/Components/TransformComponent.cpp"
"src/Scene/Components/StaticMeshComponent.cpp"
"src/Scene/Components/LightComponent.cpp"
"src/Scene/Systems/Render2dSystem.cpp"
"src/Scene/Systems/BehaviorSystem.cpp"
"src/Scene/Systems/Physics2dSystem.cpp"
"src/Scene/Systems/RenderSystem.cpp"
"src/Scene/Systems/LightSystem.cpp"
"src/Resources/TextureManager.cpp"
"src/Resources/AudioManager.cpp"
"src/Resources/FontManager.cpp"
"src/Resources/MeshLibrary.cpp"
"src/Resources/ModelImporter.cpp"
${ENGINE_HEADERS}
)

if (MODULE_EDITOR_ENABLED)
    set(ENGINE_SOURCES
    ${ENGINE_SOURCES}
    "src/Editor/ImGuiOverlay.cpp"
    "src/Editor/EditorHelpers.cpp"
    "src/Editor/Editor.cpp"
    "src/Editor/Panels/SceneHierarchyPanel.cpp"
    "src/Editor/Panels/ProfileTelemetryPanel.cpp"
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
${VENDOR_PATH}/lia/include
${VENDOR_PATH}/spdlog/include
${VENDOR_PATH}/stb/include
${VENDOR_PATH}/json/include
${VENDOR_PATH}/freetype/include
${VENDOR_PATH}/fmt/include
)

if (MODULE_EDITOR_ENABLED)
    set(ENGINE_INCLUDE_DIRS
        ${ENGINE_INCLUDE_DIRS}
        ${VENDOR_PATH}/imgui/include
    )
endif()

if (MODULE_SOUND_ENABLED)
    set(ENGINE_INCLUDE_DIRS
        ${ENGINE_INCLUDE_DIRS}
        ${VENDOR_PATH}/irrklang/include
    )
endif()

if (MODULE_3D_ENABLED)
    set(ENGINE_INCLUDE_DIRS
        ${ENGINE_INCLUDE_DIRS}
        ${VENDOR_PATH}/assimp/include
        ${PROJECT_SOURCE_DIR}/build_vendor/assimp/include # include generated config.h for assimp
    )
endif()

target_include_directories(${LIBRARY_NAME} PUBLIC ${ENGINE_INCLUDE_DIRS})

set(VENDOR_PATH_DEBUG ${PROJECT_SOURCE_DIR}/build_vendor/lib/Debug/)
set(VENDOR_PATH_RELEASE ${PROJECT_SOURCE_DIR}/build_vendor/lib/Release/)

find_library(glfw_d NAMES glfw3 PATHS ${VENDOR_PATH_DEBUG})
find_library(glad_d NAMES glad PATHS ${VENDOR_PATH_DEBUG})
find_library(spdlog_d NAMES spdlogd PATHS ${VENDOR_PATH_DEBUG})
find_library(stb_d NAMES stb PATHS ${VENDOR_PATH_DEBUG})
find_library(freetype_d NAMES freetyped PATHS ${VENDOR_PATH_DEBUG})
find_library(fmt_d NAMES fmtd PATHS ${VENDOR_PATH_DEBUG})
find_library(glad NAMES glad PATHS ${VENDOR_PATH_RELEASE})
find_library(glfw NAMES glfw3 PATHS ${VENDOR_PATH_RELEASE})
find_library(spdlog NAMES spdlog PATHS ${VENDOR_PATH_RELEASE})
find_library(stb NAMES stb PATHS ${VENDOR_PATH_RELEASE})
find_library(freetype NAMES freetype PATHS ${VENDOR_PATH_RELEASE})
find_library(fmt NAMES fmt PATHS ${VENDOR_PATH_RELEASE})

set(VENDOR
    debug ${glfw_d} optimized ${glfw}
    debug ${glad_d} optimized ${glad}
    debug ${spdlog_d} optimized ${spdlog}
    debug ${stb_d} optimized ${stb}
    debug ${freetype_d} optimized ${freetype}
    debug ${fmt_d} optimized ${fmt}
)

if (MODULE_EDITOR_ENABLED)
    find_library(imgui_d NAMES imgui imguid PATHS ${VENDOR_PATH_DEBUG})
    find_library(imgui NAMES imgui PATHS ${VENDOR_PATH_RELEASE})

    set(VENDOR
        ${VENDOR}
        debug ${imgui_d} optimized ${imgui}
        
    )
endif()

if (MODULE_SOUND_ENABLED)
    find_library(irrklang NAMES irrKlang.lib PATHS ${VENDOR_PATH}/irrklang/lib)

    set(VENDOR
        ${VENDOR}
        debug ${irrklang} optimized ${irrklang}
    )
endif()

if (MODULE_3D_ENABLED)
    find_library(assimp_d NAMES assimp-vc143-mtd PATHS ${VENDOR_PATH_DEBUG})
    find_library(zlib_d NAMES zlibstaticd PATHS ${VENDOR_PATH_DEBUG})
    find_library(assimp NAMES assimp-vc143-mt PATHS ${VENDOR_PATH_RELEASE})
    find_library(zlib NAMES zlibstatic PATHS ${VENDOR_PATH_RELEASE})

    set(VENDOR
        ${VENDOR}
        debug ${assimp_d} optimized ${assimp}
        debug ${zlib_d} optimized ${zlib}
    )
endif()

target_link_libraries(${LIBRARY_NAME} ${VENDOR})

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