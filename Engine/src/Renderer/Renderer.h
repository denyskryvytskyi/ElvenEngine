#pragma once

#include "Renderer/Camera.h"
#include "Renderer/RenderCommand.h"
#include "Shader.h"

namespace elv {

class Mesh;

class Renderer {
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(Camera& camera);
    static void EndScene();

    static void Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, const lia::mat4& modelMatrix = lia::mat4(1.0f));
    static void Submit(const SharedPtr<Shader>& shader, const SharedPtr<Mesh>& mesh, const lia::mat4& modelMatrix = lia::mat4(1.0f));

    static void OnWindowResize(std::uint32_t width, std::uint32_t height);

    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    static void SetClearColor(const lia::vec4& color) { m_clearColor = color; }
    static const lia::vec4& GetClearColor() { return m_clearColor; }

private:
    struct SceneData {
        lia::mat4 ViewProjectionMatrix;
    };

    static UniquePtr<SceneData> m_sceneData;
    static lia::vec4 m_clearColor;
};

} // namespace elv
