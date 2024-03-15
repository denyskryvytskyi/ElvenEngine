#include "Renderer/Renderer.h"

#include "Mesh.h"

namespace elv {
UniquePtr<Renderer::SceneData> Renderer::m_sceneData = MakeUniquePtr<Renderer::SceneData>();

void Renderer::Init()
{
    RenderCommand::Init();
    RenderCommand::EnableDepthTesting(true);
}

void Renderer::Shutdown()
{
    RenderCommand::Shutdown();
}

void Renderer::BeginScene(Camera& camera)
{
    m_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray, const lia::mat4& modelMatrix)
{
    shader->SetMatrix4("u_ViewProjection", m_sceneData->ViewProjectionMatrix);
    shader->SetMatrix4("u_Model", modelMatrix);

    RenderCommand::DrawIndexed(vertexArray);
}

void Renderer::Submit(const SharedPtr<Shader>& shader, const SharedPtr<Mesh>& mesh, const lia::mat4& modelMatrix)
{
    shader->SetMatrix4("u_ViewProjection", m_sceneData->ViewProjectionMatrix);
    shader->SetMatrix4("u_Model", modelMatrix);

    mesh->Draw(shader);
}

#if ASSIMP_MODE
// void Renderer::Submit(const SharedPtr<Shader>& shader, const Model& model, const lia::mat4& modelMatrix)
//{
//     shader->SetMatrix4("u_ViewProjection", m_sceneData->ViewProjectionMatrix);
//     shader->SetMatrix4("u_Model", modelMatrix);
//
//     model.Draw(shader);
// }
#endif

void Renderer::OnWindowResize(std::uint32_t width, std::uint32_t height)
{
    RenderCommand::SetViewport(0, 0, width, height);
}
} // namespace elv
