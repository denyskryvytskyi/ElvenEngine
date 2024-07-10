#pragma once

namespace elv {

class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;
    virtual void PreInit() = 0;
    virtual void Init(void* window) = 0;
    virtual void SwapBuffers() = 0;

    static UniquePtr<GraphicsContext> Create();
};

} // namespace elv
