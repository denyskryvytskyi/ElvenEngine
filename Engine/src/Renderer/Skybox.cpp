#include "Skybox.h"

#include "Core/FileSystem.h"

#include "RHI/Buffer.h"
#include "RHI/Shader.h"
#include "RHI/VertexArray.h"

#include "Renderer/Renderer.h"
#include "Resources/TextureManager.h"

#include "Platform/OpenGL/OpenGLCubemap.h"

namespace elv {

constexpr float kSkyboxVerticesNew[] = {
    // Positions
    -1.0f, 1.0f, -1.0f,  // 0
    -1.0f, -1.0f, -1.0f, // 1
    1.0f, -1.0f, -1.0f,  // 2
    1.0f, 1.0f, -1.0f,   // 3
    -1.0f, -1.0f, 1.0f,  // 4
    -1.0f, 1.0f, 1.0f,   // 5
    1.0f, -1.0f, 1.0f,   // 6
    1.0f, 1.0f, 1.0f     // 7
};

constexpr uint32_t kSkyboxIndices[] = {
    // Back face
    0, 1, 2,
    2, 3, 0,
    // Left face
    1, 4, 5,
    5, 0, 1,
    // Right face
    2, 6, 7,
    7, 3, 2,
    // Front face
    4, 6, 7,
    7, 5, 4,
    // Top face
    0, 3, 7,
    7, 5, 0,
    // Bottom face
    1, 4, 6,
    6, 2, 1
};

const lia::vec4 kResetTranslationRow = { 0.0f, 0.0f, 0.0f, 1.0f };

const std::string kDefaultFacesNames[] = {
    "skybox_right",
    "skybox_left",
    "skybox_top",
    "skybox_bottom",
    "skybox_front",
    "skybox_back"
};

const Skybox::CubemapFaceFilepaths kDefaultFacesPaths = {
    "skybox/tropical/right.png",
    "skybox/tropical/left.png",
    "skybox/tropical/top.png ",
    "skybox/tropical/bottom.png",
    "skybox/tropical/front.png",
    "skybox/tropical/back.png"
};

void Skybox::Init()
{
    m_vao = VertexArray::Create();

    // set VAO/VBO
    SharedPtr<VertexBuffer> vbo = VertexBuffer::Create((float*)&kSkyboxVerticesNew[0], static_cast<std::uint32_t>(sizeof(kSkyboxVerticesNew)));
    vbo->SetLayout({ { BufferAttributeType::Float3 } }); // position
    m_vao->AddVertexBuffer(vbo);

    SharedPtr<IndexBuffer> ebo = IndexBuffer::Create(const_cast<uint32_t*>(&kSkyboxIndices[0]), static_cast<std::uint32_t>(sizeof(kSkyboxIndices)));
    m_vao->SetIndexBuffer(ebo);

    // set default textures map
    InitCubemap(kDefaultFacesPaths);

    m_shader = ShaderManager::Load("skybox", "skybox.vert", "skybox.frag");
}

void Skybox::Draw(Renderer* renderer, const lia::mat4& viewMatrix, const lia::mat4& projectionMatrix)
{
    lia::mat4 viewWithoutTranslation = viewMatrix;
    viewWithoutTranslation[3] = kResetTranslationRow;

    const lia::mat4 viewProj = viewWithoutTranslation * projectionMatrix;

    m_vao->Bind();
    m_shader->Bind();
    m_shader->SetMatrix4("u_ViewProjection", viewProj);

    m_cubemap->Bind();

    renderer->Submit(m_vao);
}

void Skybox::SetFaces(const CubemapFaceFilepaths& filepathes)
{
    InitCubemap(filepathes);
}

void Skybox::InitCubemap(const CubemapFaceFilepaths& filepathes)
{
    // load textures for cubemap
    std::vector<TextureManager::LoadedTextureInfo> textures;
    for (int i = 0; i < filepathes.size(); ++i) {
        textures.emplace_back(textures::LoadWithoutCache(kDefaultFacesNames[i], fmt::format("{}{}", fileSystem::IMAGES_PATH, filepathes[i]), false));
    }

    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return;
    case RendererAPI::API::OpenGL:
        m_cubemap = MakeUniquePtr<OpenGLCubemap>(textures);
        return;
    }

    // free textures
    for (auto& texture : textures) {
        texture.Free();
    }
}

} // namespace elv
