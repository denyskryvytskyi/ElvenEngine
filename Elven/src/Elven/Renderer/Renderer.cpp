#include "elpch.h"
#include "Elven/Renderer/Renderer.h"

namespace Elven
{
    Ref<Renderer::SceneData> Renderer::m_SceneData = CreateRef<Renderer::SceneData>();

    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray)
    {
        shader->Bind();
        shader->SetMatrix4g("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}