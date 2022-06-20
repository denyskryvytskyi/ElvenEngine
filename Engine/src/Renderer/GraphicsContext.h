#pragma once

namespace Elven {

class GraphicsContext {
public:
    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;

    static GraphicsContext* Create(void* window);
};

} // namespace Elven
