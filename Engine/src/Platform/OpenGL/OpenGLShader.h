#pragma once

#include "Renderer/Shader.h"

namespace Elven
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& name, const ShaderProgramSource& shaderSrc);
        ~OpenGLShader();

        void Bind() const override;
        void Unbind() const override;

        void SetInteger(const std::string& name, uint32_t value) override;
        void SetVector2f(const std::string& name, float x, float y) override;
        void SetVector2f(const std::string& name, const glm::vec2& value) override;
        void SetVector3f(const std::string& name, float x, float y, float z) override;
        void SetVector3f(const std::string& name, const glm::vec3& value) override;
        void SetVector4f(const std::string& name, float x, float y, float z, float w) override;
        void SetVector4f(const std::string& name, const glm::vec4& value) override;
        void SetMatrix4(const std::string& name, const glm::mat4& matrix) override;

        int GetUniformLocation(const std::string& name);

        const std::string& GetName() const override { return m_Name; }

    private:
        void CompileProgram(const ShaderProgramSource& shaderSource);
        uint32_t CompileShader(uint32_t type, const std::string& source);

    private:
        std::unordered_map<std::string, int> m_UniformLocationCache;
        std::string m_Name;
        uint32_t m_RendererId = 0;
    };
}