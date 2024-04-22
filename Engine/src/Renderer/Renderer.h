#pragma once

#include "CameraController.h"
#include "RHI/RenderTarget.h"
#include "RHI/RendererAPI.h"
#include "RHI/Shader.h"

namespace elv {

class Mesh;
class RenderTarget;
class VertexArray;

class Renderer {
private:
    struct SceneData {
        lia::mat4 ViewProjectionMatrix;
    };

public:
    Renderer();

    void Init(const uint16_t width, const uint16_t height);
    void Shutdown();

    void BeginScene(SharedPtr<CameraController>& cameraController);
    void Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, const lia::mat4& modelMatrix = lia::mat4(1.0f));
    void Submit(const SharedPtr<Shader>& shader, const SharedPtr<Mesh>& mesh, const lia::mat4& modelMatrix = lia::mat4(1.0f));
    void Submit(const SharedPtr<VertexArray>& vertexArray, std::uint32_t indexCount = 0, const RenderTopology topology = RenderTopology::Triangles);
    void EndScene();

    void OnWindowResize(std::uint32_t width, std::uint32_t height);

    void EnableDepthTesting(bool enabled);
    void DisableByteAlignment();

    void SetClearColor(const lia::vec4& color) { m_clearColor = color; }
    const lia::vec4& GetClearColor() { return m_clearColor; }

    void EnableMSAA(bool enabled);
    bool IsMSAAEnabled() const { return m_isMSAAEnabled; }

    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    bool m_isMSAAEnabled { true };

    SceneData m_sceneData;
    lia::vec4 m_clearColor { 0.2f, 0.2f, 0.2f, 1.0f };

    UniquePtr<RendererAPI> m_rendererAPI { nullptr };

    UniquePtr<RenderTarget> m_renderTargetMultisampled { nullptr };
    UniquePtr<RenderTarget> m_renderTargetIntermidiate { nullptr };

    SharedPtr<Shader> m_screenQuadShader { nullptr };
    SharedPtr<VertexArray> m_NDCPlaneVao { nullptr };
};

} // namespace elv
