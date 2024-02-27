#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/gl.h>

namespace elv {

OpenGLShader::OpenGLShader(const std::string& name, const ShaderProgramSource& shaderSrc)
    : m_name(name)
{
    CompileProgram(shaderSrc);
}

OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(m_id);
}

void OpenGLShader::Bind() const
{
    glUseProgram(m_id);
}

void OpenGLShader::Unbind() const
{
    glUseProgram(0);
}

void OpenGLShader::SetInteger(const std::string& name, std::uint32_t value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void OpenGLShader::SetVector2f(const std::string& name, float x, float y)
{
    glUniform2f(GetUniformLocation(name), x, y);
}

void OpenGLShader::SetVector2f(const std::string& name, const lia::vec2& value)
{
    glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void OpenGLShader::SetVector3f(const std::string& name, float x, float y, float z)
{
    glUniform3f(GetUniformLocation(name), x, y, z);
}

void OpenGLShader::SetVector3f(const std::string& name, const lia::vec3& value)
{
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::SetVector4f(const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(GetUniformLocation(name), x, y, z, w);
}

void OpenGLShader::SetVector4f(const std::string& name, const lia::vec4& value)
{
    glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void OpenGLShader::SetMatrix4(const std::string& name, const lia::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix.elementsPtr());
}

int OpenGLShader::GetUniformLocation(const std::string& name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];

    int location = glGetUniformLocation(m_id, name.c_str());
    m_uniformLocationCache[name] = location;
    return location;
}

void OpenGLShader::CompileProgram(const ShaderProgramSource& shaderSource)
{
    m_id = glCreateProgram();
    std::uint32_t vs = CompileShader(GL_VERTEX_SHADER, shaderSource.VertexSource);
    std::uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, shaderSource.FragmentSource);

    glAttachShader(m_id, vs);
    glAttachShader(m_id, fs);
    glLinkProgram(m_id);

    int isLinked = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE) {
        int maxLength = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_id, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(m_id);

        EL_CORE_ERROR("Failed program linking: {0}", infoLog.data());
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

std::uint32_t OpenGLShader::CompileShader(std::uint32_t type, const std::string& source)
{
    std::uint32_t shaderId = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);

    int isCompiled = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        int maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

        glDeleteShader(shaderId);

        EL_CORE_ERROR("Failed shader compile:\n\t name: {0};\n\t type: {1};\n\t info: {2}", m_name, type == GL_VERTEX_SHADER ? "vertex" : "fragment", infoLog.data());

        return -1;
    }

    return shaderId;
}

} // namespace elv
