#include "Renderer/Renderer.h"

#include "Mesh.h"
#include "RHI/Buffer.h"

#include "Resources/TextureManager.h"

namespace elv {
Renderer::Renderer()
    : m_rendererAPI(RendererAPI::Create())
    , m_renderTargetMultisampled(RenderTarget::Create())
    , m_renderTargetIntermidiate(RenderTarget::Create())
{
}

void Renderer::Init(const uint16_t width, const uint16_t height)
{
    m_rendererAPI->SetViewport(0, 0, width, height);

    m_renderTargetMultisampled->Init(width, height, true);
    m_renderTargetIntermidiate->Init(width, height, false);

    m_screenQuadShader = ShaderManager::Load("screen_quad", "quad.vert", "quad.frag");
    m_NDCPlaneVao = VertexArray::Create();

    float quadData[] = {
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };
    SharedPtr<VertexBuffer> vbo = VertexBuffer::Create(&quadData[0], sizeof(quadData));
    vbo->SetLayout({ { BufferAttributeType::Float2 },    // pos
                     { BufferAttributeType::Float2 } }); // uv

    m_NDCPlaneVao->AddVertexBuffer(vbo);

    std::uint32_t quadIndices[] = {
        0, 1, 2, // first Triangle
        0, 2, 3  // second Triangle
    };

    SharedPtr<IndexBuffer> ebo = IndexBuffer::Create(&quadIndices[0], sizeof(quadIndices));
    m_NDCPlaneVao->SetIndexBuffer(ebo);

    m_rendererAPI->Init();

    m_postProcessor.Init(width, height);
}

void Renderer::Shutdown()
{
}

void Renderer::BeginScene(SharedPtr<CameraController>& cameraController)
{
    // 1. Render pass: draw to separate render target
    if (m_isMSAAEnabled) {
        m_renderTargetMultisampled->Bind();
    } else {
        m_renderTargetIntermidiate->Bind();
    }

    m_rendererAPI->EnableDepthTesting(true);
    m_rendererAPI->SetClearColor(Renderer::GetClearColor());
    m_rendererAPI->Clear();

    if (cameraController) {
        m_sceneData.ViewProjectionMatrix = cameraController->GetCamera().GetViewProjectionMatrix();
    }
}

void Renderer::EndScene()
{
    // 2. Render pass: blit the multisampled to the intermediate render target
    if (m_isMSAAEnabled) {
        m_renderTargetIntermidiate->Blit(m_renderTargetMultisampled);
    }

    // 3. Render pass: post-processing
    const auto postProcessedTexture = m_postProcessor.Process(this, m_renderTargetIntermidiate->GetColorTextureAttachment());

    // 4. Render pass: draw on screen
    m_rendererAPI->BindDefaultFramebuffer();

    m_rendererAPI->EnableDepthTesting(false);
    m_rendererAPI->SetClearColor(Renderer::GetClearColor());
    m_rendererAPI->ClearColorBit();

    m_screenQuadShader->Bind();
    postProcessedTexture->BindToSlot(0);

    RenderNDCQuad();
}

void Renderer::Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, const lia::mat4& modelMatrix)
{
    shader->SetMatrix4("u_ViewProjection", m_sceneData.ViewProjectionMatrix);
    shader->SetMatrix4("u_Model", modelMatrix);

    m_rendererAPI->DrawIndexed(vertexArray);
}

void Renderer::Submit(const SharedPtr<Shader>& shader, const SharedPtr<Mesh>& mesh, const lia::mat4& modelMatrix)
{
    if (mesh) {
        shader->SetMatrix4("u_ViewProjection", m_sceneData.ViewProjectionMatrix);
        shader->SetMatrix4("u_Model", modelMatrix);

        mesh->Draw(this, shader);
    } else {
        EL_CORE_ERROR("Failed to render mesh: is empty");
    }
}

void Renderer::Submit(const SharedPtr<VertexArray>& vertexArray, std::uint32_t indexCount, const RenderTopology topology)
{
    m_rendererAPI->DrawIndexed(vertexArray, indexCount);
}

void Renderer::OnWindowResize(std::uint32_t width, std::uint32_t height)
{
    m_renderTargetMultisampled->Resize(width, height);
    m_renderTargetIntermidiate->Resize(width, height);
    m_rendererAPI->SetViewport(0, 0, width, height);
    m_postProcessor.OnWindowResized(width, height);
}
void Renderer::EnableDepthTesting(bool enabled)
{
    m_rendererAPI->EnableDepthTesting(enabled);
}

void Renderer::EnableMSAA(bool enabled)
{
    m_isMSAAEnabled = enabled;
    m_rendererAPI->EnableMSAA(enabled);
}

void Renderer::RenderNDCQuad()
{
    m_rendererAPI->DrawIndexed(m_NDCPlaneVao);
}

void Renderer::DisableByteAlignment()
{
    m_rendererAPI->DisableByteAlignment();
}
void Renderer::EnableFaceCulling(bool enabled)
{
    m_rendererAPI->EnableFaceCulling(enabled);
}
void Renderer::ClearBufferBit(const BufferBitType bitType)
{
    if (bitType == BufferBitType::Color) {
        m_rendererAPI->ClearColorBit();
    } else {
        m_rendererAPI->Clear();
    }
}
} // namespace elv
