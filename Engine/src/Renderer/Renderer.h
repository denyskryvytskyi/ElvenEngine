#pragma once

#include "Renderer/RenderCommand.h"
#include "Renderer/OrthographicCamera.h"
#include "Shader.h"

namespace Elven
{
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        static void Submit(Shader* shader, const VertexArray* vertexArray, const glm::mat4& modelMatrix = glm::mat4(1.0f));

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* m_SceneData;
    };

}