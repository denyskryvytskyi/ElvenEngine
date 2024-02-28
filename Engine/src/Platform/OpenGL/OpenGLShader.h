#pragma once

#include "Renderer/Shader.h"

namespace elv {

class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& name, const ShaderProgramSource& shaderSrc);
    ~OpenGLShader() override;

    void Bind() const override;
    void Unbind() const override;

    void SetInteger(const std::string& name, std::uint32_t value) override;
    void SetFloat(const std::string& name, float value) override;
    void SetVector2f(const std::string& name, float x, float y) override;
    void SetVector2f(const std::string& name, const lia::vec2& value) override;
    void SetVector3f(const std::string& name, float x, float y, float z) override;
    void SetVector3f(const std::string& name, const lia::vec3& value) override;
    void SetVector4f(const std::string& name, float x, float y, float z, float w) override;
    void SetVector4f(const std::string& name, const lia::vec4& value) override;
    void SetMatrix4(const std::string& name, const lia::mat4& matrix) override;

    int GetUniformLocation(const std::string& name);

    const std::string& GetName() const override { return m_name; }

private:
    void CompileProgram(const ShaderProgramSource& shaderSource);
    std::uint32_t CompileShader(std::uint32_t type, const std::string& source);

private:
    std::unordered_map<std::string, int> m_uniformLocationCache;
    std::string m_name;
    std::uint32_t m_id { 0 };
};

} // namespace elv
