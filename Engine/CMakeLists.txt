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
"src/Platform/Windows/WindowsWindow.h"
"src/Platform/OpenGL/OpenGLBuffer.h"
"src/Platform/OpenGL/OpenGLContext.h"
"src/Platform/OpenGL/OpenGLRendererAPI.h"
"src/Platform/OpenGL/OpenGLShader.h"
"src/Platform/OpenGL/OpenGLVertexArray.h"
"src/Platform/OpenGL/OpenGLTexture.h"
"src/Platform/OpenGL/OpenGLRenderTarget.h"
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
"src/Renderer/RHI/Texture.cpp"
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
"src/Platform/OpenGL/OpenGLBuffer.cpp"
"src/Platform/OpenGL/OpenGLContext.cpp"
"src/Platform/OpenGL/OpenGLRendererAPI.cpp"
"src/Platform/OpenGL/OpenGLShader.cpp"
"src/Platform/OpenGL/OpenGLVertexArray.cpp"
"src/Platform/OpenGL/OpenGLTexture.cpp"
"src/Platform/OpenGL/OpenGLRenderTarget.cpp"
"src/Platform/OpenGL41/OpenGL41Buffer.cpp"
"src/Platform/OpenGL41/OpenGL41Context.cpp"
"src/Platform/OpenGL41/OpenGL41RendererAPI.cpp"
"src/Platform/OpenGL41/OpenGL41VertexArray.cpp"
"src/Platform/OpenGL41/OpenGL41Texture.cpp"
"src/Platform/OpenGL41/OpenGL41RenderTarget.cpp"
"src/Platform/Windows/WindowsInput.cpp"
"src/Platform/Windows/WindowsWindow.cpp"
"src/Platform/OpenGLCommon.cpp"
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

target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_20)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "v")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_SOURCES})

# path to dependencies
set(VENDOR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

# include dirs
set(ENGINE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ------------------
# Dependencies
# ------------------
FetchContent_Declare(
        glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw.git"
        GIT_TAG "3.4"
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        lia
        GIT_REPOSITORY "https://github.com/denyskryvytskyi/lia.git"
        GIT_TAG "main"
)
FetchContent_MakeAvailable(lia)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
        GIT_TAG "v1.14.1"
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(json URL "https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz")
FetchContent_MakeAvailable(json)

FetchContent_Declare(
        freetype
        GIT_REPOSITORY "https://github.com/freetype/freetype.git"
        GIT_TAG "VER-2-13-2"
)

set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(freetype)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
        GIT_TAG "10.2.1"
)
FetchContent_MakeAvailable(fmt)

# zlib must be built before assimp
FetchContent_Declare(
        zlib
        GIT_REPOSITORY "https://github.com/madler/zlib.git"
        GIT_TAG "v1.3.1"
)
FetchContent_MakeAvailable(zlib)
# ----

FetchContent_Declare(
        assimp
        GIT_REPOSITORY "https://github.com/assimp/assimp.git"
        GIT_TAG "v5.3.1"
)

# Manually tell assimp where to find zlib
set(ZLIB_FOUND ON)
set(ZLIB_INCLUDE_DIR ${zlib_SOURCE_DIR} CACHE PATH "zlib include directory" FORCE)
set(ZLIB_LIBRARIES zlibstatic CACHE STRING "zlib list of libraries to link" FORCE)
set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(assimp)


FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG "docking"
)

FetchContent_MakeAvailable(imgui)


set(IRRKLANG_INSTALL_DIR ${CMAKE_BINARY_DIR}/_deps/irrklang)

FetchContent_Declare(
    irrklang
	URL "http://www.ambiera.at/downloads/irrKlang-64bit-1.5.0.zip"
    SOURCE_DIR ${IRRKLANG_INSTALL_DIR}
)

FetchContent_GetProperties(irrklang)

if(NOT irrklang_POPULATED)
	FetchContent_Populate(irrklang)
	set(IRRKLANG_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(IRRKLANG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(IRRKLANG_BUILD_TESTS OFF CACHE BOOL "" FORCE)
endif()

add_subdirectory(${VENDOR_PATH}/GLAD)
add_subdirectory(${VENDOR_PATH}/stb)


if (MODULE_EDITOR_ENABLED)
    set(IMGUI_DIR ${imgui_SOURCE_DIR})

    set(IMGUI_PUBLIC_HEADERS
        ${IMGUI_DIR}/imconfig.h
        ${IMGUI_DIR}/imgui.h
        ${IMGUI_DIR}/imgui_internal.h # not actually public, but users might need it
        ${IMGUI_DIR}/imstb_rectpack.h
        ${IMGUI_DIR}/imstb_textedit.h
        ${IMGUI_DIR}/imstb_truetype.h
        ${IMGUI_DIR}/misc/cpp/imgui_stdlib.h
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
        ${IMGUI_DIR}/backends/imgui_impl_glfw.h
    )

    set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    )

    target_include_directories(${LIBRARY_NAME}
        PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends
    )

    target_sources(${LIBRARY_NAME} PRIVATE ${IMGUI_SOURCES})
endif()


target_include_directories(${LIBRARY_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (MODULE_SOUND_ENABLED)
    if (APPLE)
        messsage(FATAL_ERROR "Sound module is not yet supported on MacOS. Please disable this option for now")
    else()
        target_include_directories(${LIBRARY_NAME} PUBLIC ${irrklang_SOURCE_DIR}/include)
        # irrklang is marked here STATIC to avoid IMPORTED_IMPLIB not found error on Windows
        # the dlls are copied manually in the post build step
        add_library(irrklang STATIC IMPORTED)
        set_target_properties(irrklang PROPERTIES IMPORTED_LOCATION "${IRRKLANG_INSTALL_DIR}/lib/Winx64-visualStudio/irrKlang.lib")
        target_link_libraries(${LIBRARY_NAME} irrklang)
    endif()
endif()


target_link_libraries(${LIBRARY_NAME} glfw spdlog freetype fmt glad stb)
target_link_libraries(${LIBRARY_NAME} nlohmann_json::nlohmann_json)
target_include_directories(${LIBRARY_NAME} PUBLIC ${lia_SOURCE_DIR}/include)

target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (MODULE_3D_ENABLED)
    target_link_libraries(${LIBRARY_NAME} zlib assimp)
endif()

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