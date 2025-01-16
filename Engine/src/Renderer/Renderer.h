#pragma once

#include "CameraController.h"
#include "PostProcessor.h"

#include "RHI/RenderTarget.h"
#include "RHI/RendererAPI.h"
#include "RHI/Shader.h"

#include "Scene/Components/LightComponent.h"

#include "Skybox.h"

namespace elv {

class Mesh;
class RenderTarget;
class VertexArray;

class Renderer {
    friend class PostProcessor;

public:
    enum class BufferBitType {
        Color,
        Depth,
        ColorDepth
    };

private:
    struct SceneData {
        lia::mat4 ViewProjectionMatrix;
        lia::mat4 ViewMatrix;
        lia::mat4 ProjectionMatrix;
    };

public:
    Renderer();

    void Init(const uint16_t width, const uint16_t height);
    void Shutdown();

    void BeginScene(SharedPtr<CameraController>& cameraController);
    void Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, const lia::mat4& modelMatrix = lia::mat4(1.0f));
    void Submit(const SharedPtr<Shader>& shader, const SharedPtr<Mesh>& mesh, const lia::mat4& modelMatrix = lia::mat4(1.0f));
    void Submit(const SharedPtr<VertexArray>& vertexArray, std::uint32_t indexCount = 0, const RenderTopology topology = RenderTopology::Triangles);
    void SubmitArrays(const SharedPtr<VertexArray>& vertexArray, std::uint32_t verticesAmount, const RenderTopology topology = RenderTopology::Triangles);
    void EndScene();

    void OnWindowResize(std::uint32_t width, std::uint32_t height);

    void EnableDepthTesting(bool enabled);
    void DisableByteAlignment();
    void EnableFaceCulling(bool enabled);
    void ClearBufferBit(const BufferBitType colorBit);

    void EnableSkybox(bool enabled);
    bool IsSkyboxEnabled() const { return m_isSkyboxEnabled; }
    void SetSkyboxFaces(const Skybox::CubemapFaceFilepaths& filepathes);

    void SetClearColor(const lia::vec4& color) { m_clearColor = color; }
    const lia::vec4& GetClearColor() { return m_clearColor; }

    void EnableMSAA(bool enabled);
    bool IsMSAAEnabled() const { return m_isMSAAEnabled; }

    void EnableBlinnPhong(const bool enabled) { m_isBlinnPhongEnabled = enabled; }
    bool IsBlinnPhongEnabled() const { return m_isBlinnPhongEnabled; }

    PostProcessor& GetPostProcessor() { return m_postProcessor; }

    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

private:
    void RenderNDCQuad();

private:
    bool m_isMSAAEnabled { true };
    bool m_isBlinnPhongEnabled { false };
    bool m_isSkyboxEnabled { true };

    SceneData m_sceneData;
    lia::vec4 m_clearColor { 0.2f, 0.2f, 0.2f, 1.0f };

    PostProcessor m_postProcessor;

    UniquePtr<RendererAPI> m_rendererAPI { nullptr };

    UniquePtr<RenderTarget> m_renderTargetMultisampled { nullptr };
    UniquePtr<RenderTarget> m_renderTargetIntermidiate { nullptr };

    SharedPtr<Shader> m_screenQuadShader { nullptr };
    SharedPtr<VertexArray> m_NDCPlaneVao { nullptr };

    Skybox m_skybox;
};

} // namespace elv
