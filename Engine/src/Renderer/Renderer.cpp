#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

namespace Elven
{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

    void Renderer::Init()
    {
        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown()
    {
        RenderCommand::Shutdown();

        if (m_SceneData)
        {
            delete m_SceneData;
        }
    }

    void Renderer::BeginScene(Camera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(Shader* shader, const VertexArray* vertexArray, const lia::mat4& modelMatrix)
    {
        shader->Bind();
        shader->SetMatrix4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
        shader->SetMatrix4("u_Model", modelMatrix);

        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }
}