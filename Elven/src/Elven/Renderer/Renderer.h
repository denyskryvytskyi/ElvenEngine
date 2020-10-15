#pragma once

#include "Elven/Renderer/RenderCommand.h"

namespace Elven
{
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene();
        static void EndScene();

        static void Submit(const Ref<VertexArray>& vertexArray);

        static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    };

}