#pragma once

namespace elv {

class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;
    virtual void PreInit() = 0;
    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;

    static UniquePtr<GraphicsContext> Create(void* window);
};

} // namespace elv
