#include "elpch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Elven
{
    OpenGLShader::OpenGLShader(const std::string& name, const ShaderProgramSource& shaderSrc)
        : m_Name(name)
    {
        CompileProgram(shaderSrc);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererId);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererId);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetInteger(const std::string& name, uint32_t value)
    {
        glUniform1i(GetUniformLocation(name), value);
    }

    void OpenGLShader::SetVector2f(const std::string& name, float x, float y)
    {
        glUniform2f(GetUniformLocation(name), x, y);
    }

    void OpenGLShader::SetVector2f(const std::string& name, const glm::vec2& value)
    {
        glUniform2f(GetUniformLocation(name), value.x, value.y);
    }

    void OpenGLShader::SetVector3f(const std::string& name, float x, float y, float z)
    {
        glUniform3f(GetUniformLocation(name), x, y, z);
    }

    void OpenGLShader::SetVector3f(const std::string& name, const glm::vec3& value)
    {
        glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
    }

    void OpenGLShader::SetVector4f(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(GetUniformLocation(name), x, y, z, w);
    }

    void OpenGLShader::SetVector4f(const std::string& name, const glm::vec4& value)
    {
        glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetMatrix4(const std::string& name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    //////////////////////////

    int OpenGLShader::GetUniformLocation(const std::string& name)
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];

        int location = glGetUniformLocation(m_RendererId, name.c_str());
        m_UniformLocationCache[name] = location;
        return location;
    }

    void OpenGLShader::CompileProgram(const ShaderProgramSource& shaderSource)
    {
        m_RendererId = glCreateProgram();
        uint32_t vs = CompileShader(GL_VERTEX_SHADER, shaderSource.VertexSource);
        uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, shaderSource.FragmentSource);

        glAttachShader(m_RendererId, vs);
        glAttachShader(m_RendererId, fs);
        glLinkProgram(m_RendererId);

        int isLinked = 0;
        glGetProgramiv(m_RendererId, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            int maxLength = 0;
            glGetProgramiv(m_RendererId, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_RendererId, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(m_RendererId);

            EL_CORE_ERROR("Failed program linking: {0}", infoLog.data());
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    uint32_t OpenGLShader::CompileShader(uint32_t type, const std::string& source)
    {
        uint32_t shaderId = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shaderId, 1, &src, nullptr);
        glCompileShader(shaderId);

        int isCompiled = 0;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

        if (isCompiled == GL_FALSE)
        {
            int maxLength = 0;
            glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shaderId);

            EL_CORE_ERROR("Failed shader compile: {0} {1} {2}", type == GL_VERTEX_SHADER ? "vertex" : "fragment", " shader:", infoLog.data());

            return -1;
        }

        return shaderId;
    }
}