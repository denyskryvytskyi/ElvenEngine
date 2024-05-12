#pragma once

#include "Renderer/RHI/GraphicsContext.h"

struct GLFWwindow;

namespace elv {

class OpenGLContext : public GraphicsContext {
public:
    OpenGLContext();

    void Init(void* wnd) override;
    void SwapBuffers() override;
    void PreInit() override;

private:
    GLFWwindow* m_windowHandle { nullptr };
};

} // namespace elv
