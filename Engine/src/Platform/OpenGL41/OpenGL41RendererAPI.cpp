#include "Platform/OpenGL41/OpenGL41RendererAPI.h"
#include "Platform/OpenGL41/OpenGL41Check.h"
#include "Platform/OpenGLCommon.h"

#include <glad/gl.h>

#include <string>

namespace elv {

void glCheckError(
    const char* file,
    int line,
    const char* expression)
{
    GLenum errorCode;

    do {
        errorCode = glGetError();

        if (errorCode != GL_NO_ERROR) {
            std::string fileString = file;
            std::string error = "Unknown error = " + std::to_string(errorCode);
            std::string description = "No description";

            switch (errorCode) {
            case GL_INVALID_ENUM: {
                error = "GL_INVALID_ENUM";
                description = "An unacceptable value has been specified for an enumerated argument.";
                break;
            }

            case GL_INVALID_VALUE: {
                error = "GL_INVALID_VALUE";
                description = "A numeric argument is out of range.";
                break;
            }

            case GL_INVALID_OPERATION: {
                error = "GL_INVALID_OPERATION";
                description = "The specified operation is not allowed in the current state.";
                break;
            }

            case GL_STACK_OVERFLOW: {
                error = "GL_STACK_OVERFLOW";
                description = "This command would cause a stack overflow.";
                break;
            }

            case GL_STACK_UNDERFLOW: {
                error = "GL_STACK_UNDERFLOW";
                description = "This command would cause a stack underflow.";
                break;
            }

            case GL_OUT_OF_MEMORY: {
                error = "GL_OUT_OF_MEMORY";
                description = "There is not enough memory left to execute the command.";
                break;
            }
            }

            std::string finalMessage = "An internal OpenGL call failed in " + fileString + "(" + std::to_string(line) + ").\nExpression:\n   " + expression + "\nError description:\n   " + error + "\n   " + description + "\n";

            // Log the error
            EL_CORE_ERROR(finalMessage);

            throw std::runtime_error(finalMessage);
        }
    } while (errorCode != GL_NO_ERROR);
}

////////////////////////////////////////////////////////

void OpenGL41RendererAPI::Init()
{
    // blending
    glCheck(glEnable(GL_BLEND));
    glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void OpenGL41RendererAPI::SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height)
{
    glCheck(glViewport(x, y, width, height));
}

void OpenGL41RendererAPI::SetClearColor(const lia::vec4& color)
{
    glCheck(glClearColor(color.x, color.y, color.z, color.w));
}
void OpenGL41RendererAPI::Clear()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void OpenGL41RendererAPI::ClearColorBit()
{
    glCheck(glClear(GL_COLOR_BUFFER_BIT));
}

void OpenGL41RendererAPI::EnableDepthTesting(bool enabled)
{
    if (enabled) {
        glCheck(glEnable(GL_DEPTH_TEST));
        glCheck(glDepthFunc(GL_LEQUAL));
    } else {
        glCheck(glDisable(GL_DEPTH_TEST));
    }
}

void OpenGL41RendererAPI::EnableMSAA(bool enabled)
{
    if (enabled) {
        glCheck(glEnable(GL_MULTISAMPLE));
    } else {
        glCheck(glDisable(GL_MULTISAMPLE));
    }
}

void OpenGL41RendererAPI::EnableFaceCulling(bool enabled)
{
    if (enabled) {
        glCheck(glEnable(GL_CULL_FACE));
    } else {
        glCheck(glDisable(GL_CULL_FACE));
    }
}

void OpenGL41RendererAPI::DisableByteAlignment()
{
    glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
}

void OpenGL41RendererAPI::BindDefaultFramebuffer()
{
    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void OpenGL41RendererAPI::DrawIndexed(const SharedPtr<VertexArray>& vertexArray, const std::uint32_t indexCount, const RenderTopology topology)
{
    const std::uint32_t count = indexCount ? indexCount : vertexArray->GetIndexCount();
    vertexArray->Bind();
    glCheck(glDrawElements(OpenGL::GetTopology(topology), count, GL_UNSIGNED_INT, nullptr));
}

} // namespace elv
