#include "Mesh.h"

#include "RHI/Buffer.h"
#include "RHI/Shader.h"
#include "RHI/Texture.h"
#include "RHI/VertexArray.h"
#include "Renderer.h"

namespace elv {

Mesh::Mesh()
    : m_vao(VertexArray::Create())
{
}

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices, const RenderTopology topology)
    : m_topology(topology)
    , m_vertices(vertices)
    , m_indices(indices)
    , m_vao(VertexArray::Create())
{
    SetupMesh();
}

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices, const std::vector<MeshTexture>& texturesInfo)
    : m_vertices(vertices)
    , m_indices(indices)
    , m_vao(VertexArray::Create())
{
    SetupMesh();
    SetupMaterial(texturesInfo);
}

void Mesh::SetInfo(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices, const std::vector<MeshTexture>& texturesInfo)
{
    m_vertices = vertices;
    m_indices = indices;

    SetupMesh();
    SetupMaterial(texturesInfo);
}

void Mesh::AddSubmesh(Mesh&& submesh)
{
    m_submeshes.emplace_back(std::move(submesh));
}

void Mesh::SetMaterialTexture(const Material::TextureSlot slot, const std::string& name, const std::string& path, bool async)
{
    m_material.SetTexture(slot, name, path, async);
}

void Mesh::LoadTextures(const std::string& dir, const bool async)
{
    m_material.LoadTextures(dir, async);
    for (auto& submesh : m_submeshes) {
        submesh.LoadTextures(dir, async);
    }
}

void Mesh::Draw(Renderer* renderer, const SharedPtr<Shader>& shader) const
{
    m_vao->Bind();
    m_material.ApplyMaterial(shader);
    renderer->Submit(m_vao, 0, m_topology);

    // draw submeshes
    for (auto& submesh : m_submeshes) {
        submesh.Draw(renderer, shader);
    }
}

void Mesh::SetupMesh()
{
    SharedPtr<VertexBuffer> vbo = VertexBuffer::Create((float*)&m_vertices[0], static_cast<std::uint32_t>(m_vertices.size()) * static_cast<std::uint32_t>(sizeof(MeshVertex)));
    vbo->SetLayout({ { BufferAttributeType::Float3 },    // pos
                     { BufferAttributeType::Float3 },    // normal
                     { BufferAttributeType::Float2 } }); // uv

    m_vao->AddVertexBuffer(vbo);

    SharedPtr<IndexBuffer> ebo = IndexBuffer::Create(&m_indices[0], static_cast<std::uint32_t>(m_indices.size()));
    m_vao->SetIndexBuffer(ebo);
}

void Mesh::SetupMaterial(const std::vector<MeshTexture>& texturesInfo)
{
    for (const auto& textureInfo : texturesInfo) {
        m_material.SetTexture(textureInfo.slotType, textureInfo.name);
    }
}
} // namespace elv
