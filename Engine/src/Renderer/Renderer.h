#pragma once

#include "Renderer/Camera.h"
#include "Renderer/RenderCommand.h"
#include "Shader.h"

#include "Model.h"

namespace elv {

class Mesh;

class Renderer {
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(Camera& camera);
    static void EndScene();

    static void Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, const lia::mat4& modelMatrix = lia::mat4(1.0f));
    static void Submit(const SharedPtr<Shader>& shader, const UniquePtr<Mesh>& mesh, const lia::mat4& modelMatrix = lia::mat4(1.0f));

#if ASSIMP_MODE
    static void Submit(const SharedPtr<Shader>& shader, const Model& model, const lia::mat4& modelMatrix = lia::mat4(1.0f));
#endif

    static void OnWindowResize(std::uint32_t width, std::uint32_t height);

    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    struct SceneData {
        lia::mat4 ViewProjectionMatrix;
    };

    static UniquePtr<SceneData> m_sceneData;
};

} // namespace elv
