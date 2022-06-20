#pragma once

#include <lia/lia.h>

#include "Renderer/RendererAPI.h"

namespace Elven {

class RenderCommand {
public:
    static void Init()
    {
        s_RendererAPI->Init();
    }

    static void Shutdown()
    {
        if (s_RendererAPI) {
            delete s_RendererAPI;
        }
    }

    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        s_RendererAPI->SetViewport(x, y, width, height);
    }

    static void SetClearColor(const lia::vec4& color)
    {
        s_RendererAPI->SetClearColor(color);
    }

    static void Clear()
    {
        s_RendererAPI->Clear();
    }

    static void EnableDepthTesting(bool enable)
    {
        s_RendererAPI->EnableDepthTesting(enable);
    }

    static void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, uint32_t indexCount = 0)
    {
        s_RendererAPI->DrawIndexed(vertexArray, indexCount);
    }

private:
    static RendererAPI* s_RendererAPI;
};

} // namespace Elven
