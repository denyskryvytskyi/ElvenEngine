#include "elpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Elven
{
    void OpenGLMessageCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH: EL_CORE_CRITICAL(message); return;
        case GL_DEBUG_SEVERITY_MEDIUM: EL_CORE_ERROR(message); return;
        case GL_DEBUG_SEVERITY_LOW: EL_CORE_WARN(message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: EL_CORE_INFO(message); return;
        }

        EL_CORE_ASSERT(false, "Unknown severity level!");
    }

    ////////////////////////////////////////////////////////

    void OpenGLRendererAPI::Init()
    {
    #ifdef EL_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLMessageCallback, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
    #endif
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const gdm::vec4& color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
    }
    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
    {
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}
