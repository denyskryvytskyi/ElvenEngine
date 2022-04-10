#pragma once

#include <lia/lia.h>

namespace Elven
{
    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetInteger(const std::string& name, uint32_t value) = 0;
        virtual void SetVector2f(const std::string& name, float x, float y) = 0;
        virtual void SetVector2f(const std::string& name, const lia::vec2& value) = 0;
        virtual void SetVector3f(const std::string& name, float x, float y, float z) = 0;
        virtual void SetVector3f(const std::string& name, const lia::vec3& value) = 0;
        virtual void SetVector4f(const std::string& name, float x, float y, float z, float w) = 0;
        virtual void SetVector4f(const std::string& name, const lia::vec4& value) = 0;
        virtual void SetMatrix4(const std::string& name, const lia::mat4& matrix) = 0;

        virtual const std::string& GetName() const = 0;

        static Shader* Create(const std::string& name, const ShaderProgramSource& shaderSrc);
    };

    class ShaderManager
    {
    public:
        Shader* Load(std::string name, const std::string& vShaderFile, const std::string& fShaderFile);

        Shader* Get(const std::string& name);

    private:
        std::string ReadFile(const std::string& shaderFile);

    private:
        std::unordered_map<std::string, Shader*> m_Shaders;
    };
}