#pragma once

#include "Renderer/RHI/GraphicsContext.h"

struct GLFWwindow;

namespace elv {

class OpenGL41Context : public GraphicsContext {
public:
    OpenGL41Context(GLFWwindow* windowHandle);

    void Init() override;
    void SwapBuffers() override;
    void PreInit() override;

private:
    GLFWwindow* m_windowHandle { nullptr };
};

} // namespace elv
