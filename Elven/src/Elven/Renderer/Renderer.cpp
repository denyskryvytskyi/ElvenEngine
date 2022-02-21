#include "elpch.h"
#include "Elven/Renderer/Renderer.h"

namespace Elven
{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::Shutdown()
    {
        RenderCommand::Shutdown();

        if (m_SceneData)
        {
            delete m_SceneData;
        }
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(Shader* shader, const VertexArray* vertexArray, const gdm::mat4& modelMatrix)
    {
        shader->Bind();
        shader->SetMatrix4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
        shader->SetMatrix4("u_Model", modelMatrix);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}