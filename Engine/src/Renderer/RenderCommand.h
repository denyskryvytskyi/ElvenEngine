#pragma once

#include "Renderer/RHI/RendererAPI.h"

namespace elv {

class RenderCommand {
public:
    static void Init()
    {
        s_RendererAPI->Init();
    }

    static void Shutdown()
    {
    }

    static void SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height)
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

    static void ClearColorBit()
    {
        s_RendererAPI->ClearColorBit();
    }

    static void EnableDepthTesting(bool enable)
    {
        s_RendererAPI->EnableDepthTesting(enable);
    }

    static void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, std::uint32_t indexCount = 0, const RenderTopology topology = RenderTopology::Triangles)
    {
        s_RendererAPI->DrawIndexed(vertexArray, indexCount, topology);
    }

    static void DisableByteAlignment()
    {
        s_RendererAPI->DisableByteAlignment();
    }

    static void EnableMSAA(bool enable)
    {
        s_RendererAPI->EnableMSAA(enable);
    }

private:
    static UniquePtr<RendererAPI> s_RendererAPI;
};

} // namespace elv
