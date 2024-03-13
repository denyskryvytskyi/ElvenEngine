#include "Mesh.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "Texture2D.h"
#include "VertexArray.h"

#include "Core/StringId.h"
#include "Events/EventManager.h"
#include "Events/TextureEvent.h"
#include "Resources/TextureManager.h"

namespace elv {

std::pair<const char*, int> GetTextureBindInfo(const MeshTextureType& type)
{
    switch (type) {
    case MeshTextureType::Specular:
        return std::make_pair<const char*, int>("texture_specular", 1);
    case MeshTextureType::Emission:
        return std::make_pair<const char*, int>("texture_emission", 2);
    case MeshTextureType::Opacity:
        return std::make_pair<const char*, int>("texture_opacity", 3);
    }

    return std::make_pair<const char*, int>("texture_diffuse", 0);
}

Mesh::Mesh()
    : m_vao(elv::VertexArray::Create())
{
}

void Mesh::LoadTexturesAsync(const std::string& dir)
{
    for (auto& t : m_textures) {
        if (!t.texturePtr) {
            t.LoadTexture(dir);
        }
    }
}

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices)
    : m_vertices(vertices)
    , m_indices(indices)
    , m_vao(elv::VertexArray::Create())
{
    SetupMesh();
}

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices, std::vector<MeshTexture>&& textures)
    : m_vertices(vertices)
    , m_indices(indices)
    , m_textures(textures)
    , m_vao(elv::VertexArray::Create())
{
    SetupMesh();
}

void Mesh::Draw(const SharedPtr<Shader>& shader) const
{
    int textureUnit = 0;
    for (auto& meshTexture : m_textures) {
        if (!meshTexture.texturePtr)
            continue;

        const auto info = GetTextureBindInfo(meshTexture.type);

        meshTexture.texturePtr->BindToUnit(info.second);

        shader->SetInteger(fmt::format("u_Material.{}", info.first), info.second);
        ++textureUnit;
    }

    m_vao->Bind();
    RenderCommand::DrawIndexed(m_vao);

    for (auto& meshTexture : m_textures) {
        if (!meshTexture.texturePtr)
            continue;

        meshTexture.texturePtr->Unbind();
    }
}

void Mesh::SetTextures(std::vector<MeshTexture>&& textures)
{
    m_textures = std::move(textures);
}

void Mesh::SetupMesh()
{
    elv::SharedPtr<elv::VertexBuffer> vbo = elv::VertexBuffer::Create((float*)&m_vertices[0], static_cast<std::uint32_t>(m_vertices.size()) * static_cast<std::uint32_t>(sizeof(MeshVertex)));
    vbo->SetLayout({ { elv::BufferAttributeType::Float3 },    // pos
                     { elv::BufferAttributeType::Float3 },    // normal
                     { elv::BufferAttributeType::Float2 } }); // uv

    m_vao->AddVertexBuffer(vbo);

    elv::SharedPtr<elv::IndexBuffer> ebo = elv::IndexBuffer::Create(&m_indices[0], static_cast<std::uint32_t>(m_indices.size()));
    m_vao->SetIndexBuffer(ebo);
}

void MeshTexture::LoadTexture(const std::string& textureDir)
{
    if (texturePtr)
        return;

    auto readyTexturePtr = textures::Get(textureName);
    if (readyTexturePtr) {
        texturePtr = readyTexturePtr;
    } else {
        textures::Load(textureName, fmt::format("{}/{}", textureDir, textureName));

        events::Subscribe<events::TextureLoadedEvent>([&](const events::TextureLoadedEvent& e) mutable {
            if (e.textureName == textureName) {
                texturePtr = textures::Get(textureName);
            }
        },
                                                      string_id(textureName));
    }
}
} // namespace elv
