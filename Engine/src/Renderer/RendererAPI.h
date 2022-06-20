#pragma once

#include "Renderer/VertexArray.h"

namespace Elven {

class RendererAPI {
public:
    enum class API {
        None = 0,
        OpenGL = 1,
    };

public:
    virtual ~RendererAPI() = default;

    virtual void Init() = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor(const lia::vec4& color) = 0;
    virtual void Clear() = 0;
    virtual void EnableDepthTesting(bool enabled) = 0;

    virtual void DrawIndexed(const SharedPtr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

    static API GetAPI() { return s_API; }
    static RendererAPI* Create();

private:
    static API s_API;
};

} // namespace Elven
