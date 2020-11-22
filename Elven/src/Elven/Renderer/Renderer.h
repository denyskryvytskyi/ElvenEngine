#pragma once

#include "Elven/Renderer/RenderCommand.h"
#include "Elven/Renderer/OrthographicCamera.h"
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

        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    private:
        struct SceneData
        {
            gdm::mat4 ViewProjectionMatrix;
        };

        static Ref<SceneData> m_SceneData;
    };

}