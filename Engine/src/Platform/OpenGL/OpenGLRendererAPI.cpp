#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/gl.h>

namespace elv {

void OpenGLMessageCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam)
{
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        EL_CORE_CRITICAL(message);
        return;
    case GL_DEBUG_SEVERITY_MEDIUM:
        EL_CORE_ERROR(message);
        return;
    case GL_DEBUG_SEVERITY_LOW:
        EL_CORE_WARN(message);
        return;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        EL_CORE_INFO(message);
        return;
    }

    EL_CORE_ASSERT(false, "Unknown severity level!");
}

////////////////////////////////////////////////////////

void OpenGLRendererAPI::Init()
{
#ifdef DEBUG_MODE
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRendererAPI::SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height)
{
    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const lia::vec4& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
}
void OpenGLRendererAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::EnableDepthTesting(bool enabled)
{
    if (enabled) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void OpenGLRendererAPI::DisableByteAlignment()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void OpenGLRendererAPI::DrawIndexed(const SharedPtr<VertexArray>& vertexArray, std::uint32_t indexCount)
{
    const std::uint32_t count = indexCount ? indexCount : vertexArray->GetIndexCount();
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

} // namespace elv
