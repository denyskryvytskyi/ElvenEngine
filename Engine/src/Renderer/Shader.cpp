
#include "Renderer/Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/Renderer.h"

#include "Core/FileSystem.h"

namespace Elven {
Shader* Shader::Create(const std::string& name, const ShaderProgramSource& shaderSrc)
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return new OpenGLShader(name, shaderSrc);
    }

    EL_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

//////////////////////////////////////////////////////////

std::unordered_map<std::string, Shader*> ShaderManager::m_Shaders;

Shader* ShaderManager::Load(std::string name, const std::string& vShaderFile, const std::string& fShaderFile)
{
    ShaderProgramSource shaderProgram;

    shaderProgram.VertexSource = ReadFile(vShaderFile);
    shaderProgram.FragmentSource = ReadFile(fShaderFile);

    auto shader = Shader::Create(name, shaderProgram);
    m_Shaders[name] = shader;

    return shader;
}

Shader* ShaderManager::Get(const std::string& name)
{
    return m_Shaders[name];
}

std::string ShaderManager::ReadFile(const std::string& shaderFile)
{
    const std::string shaderPath = FileSystem::GetShadersPath() + shaderFile;
    std::string result;
    std::ifstream in(shaderPath, std::ios::in || std::ios::binary);
    if (in) {
        in.seekg(0, in.end);
        result.resize(in.tellg());
        in.seekg(0, in.beg);
        in.read(&result[0], result.size());
        in.close();
    } else {
        EL_ASSERT(false, "Could not open file!");
    }

    return result;
}
} // namespace Elven
