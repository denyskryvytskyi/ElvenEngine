#pragma once

#include "VertexArray.h"

#include "Renderer/RenderTopology.h"

namespace elv {

class RendererAPI {
public:
    enum class API {
        None = 0,
        OpenGL = 1,
    };

public:
    virtual ~RendererAPI() = default;

    virtual void Init() = 0;
    virtual void SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height) = 0;
    virtual void SetClearColor(const lia::vec4& color) = 0;
    virtual void Clear() = 0;
    virtual void ClearColorBit() = 0;
    virtual void EnableDepthTesting(bool enabled) = 0;
    virtual void EnableMSAA(bool enabled) = 0;
    virtual void EnableFaceCulling(bool enabled) = 0;
    virtual void DisableByteAlignment() = 0;
    virtual void BindDefaultFramebuffer() = 0;

    virtual void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, const std::uint32_t indexCount = 0, const RenderTopology topology = RenderTopology::Triangles) = 0;

    static API GetAPI() { return s_API; }
    static UniquePtr<RendererAPI> Create();

private:
    static API s_API;
};

} // namespace elv
