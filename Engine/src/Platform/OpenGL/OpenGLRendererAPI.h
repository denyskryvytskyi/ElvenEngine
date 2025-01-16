#pragma once

#include "Renderer/RHI/RendererAPI.h"

namespace elv {

class OpenGLRendererAPI : public RendererAPI {
    void Init() override;
    void SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) override;

    void SetClearColor(const lia::vec4& color) override;
    void Clear() override;
    void ClearColorBit() override;

    void EnableDepthTesting(bool enabled) override;
    void EnableDepthMask(bool enabled) override;
    void EnableMSAA(bool enabled) override;
    void EnableFaceCulling(bool enabled) override;

    void DisableByteAlignment() override;

    void BindDefaultFramebuffer() override;

    void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, const std::uint32_t indexCount = 0, const RenderTopology topology = RenderTopology::Triangles) override;
    void DrawArrays(const SharedPtr<VertexArray>& vertexArray, const std::uint32_t verticesAmount = 0, const RenderTopology topology = RenderTopology::Triangles) override;
};

} // namespace elv
