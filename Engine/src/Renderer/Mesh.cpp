#include "Mesh.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "Texture2D.h"
#include "VertexArray.h"

namespace elv {

const char* GetTextureTypeName(const MeshTextureType& type)
{
    switch (type) {
    case MeshTextureType::Specular:
        return "texture_specular";
    }

    return "texture_diffuse";
}

Mesh::Mesh()
    : m_vao(elv::VertexArray::Create())
{
}

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices)
    : m_vertices(vertices)
    , m_indices(indices)
    , m_vao(elv::VertexArray::Create())
{
    SetupMesh();
}

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices, const std::vector<MeshTexture>& textures)
    : m_vertices(vertices)
    , m_indices(indices)
    , m_textures(textures)
    , m_vao(elv::VertexArray::Create())
{
    SetupMesh();
}

void Mesh::Draw(const SharedPtr<elv::Shader>& shader)
{
    std::uint32_t diffuseIndex = 1;
    std::uint32_t specularIndex = 1;

    int textureUnit = 0;
    for (auto& meshTexture : m_textures) {
        meshTexture.texture->BindToUnit(textureUnit);

        std::uint32_t textureNumber = 0;
        switch (meshTexture.type) {
        case MeshTextureType::Diffuse:
            textureNumber = diffuseIndex++;
            break;
        case MeshTextureType::Specular:
            textureNumber = specularIndex++;
            break;
        }

        shader->SetInteger(fmt::format("u_Material.{}{}", GetTextureTypeName(meshTexture.type), textureNumber), textureUnit);
        ++textureUnit;
    }

    m_vao->Bind();
    RenderCommand::DrawIndexed(m_vao);
}

void Mesh::SetTextures(const std::vector<MeshTexture>& textures)
{
    m_textures = textures;
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
} // namespace elv
