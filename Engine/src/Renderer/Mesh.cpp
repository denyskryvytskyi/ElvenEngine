#include "Mesh.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "Texture2D.h"
#include "VertexArray.h"

namespace elv {

Mesh::Mesh()
    : m_vao(elv::VertexArray::Create())
{
}

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices, const RenderTopology topology)
    : m_topology(topology)
    , m_vertices(vertices)
    , m_indices(indices)
    , m_vao(elv::VertexArray::Create())
{
    SetupMesh();
}

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices, const std::vector<MeshTexture>& texturesInfo)
    : m_vertices(vertices)
    , m_indices(indices)
    , m_vao(elv::VertexArray::Create())
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

void Mesh::LoadTextures(const std::string& dir, const bool async)
{
    m_material.LoadTextures(dir, async);
    for (auto& submesh : m_submeshes) {
        submesh.LoadTextures(dir, async);
    }
}

void Mesh::Draw(const SharedPtr<Shader>& shader) const
{
    m_vao->Bind();
    m_material.ApplyMaterial(shader);
    RenderCommand::DrawIndexed(m_vao, 0, m_topology);

    // draw submeshes
    for (auto& submesh : m_submeshes) {
        submesh.Draw(shader);
    }
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

void Mesh::SetupMaterial(const std::vector<MeshTexture>& texturesInfo)
{
    for (const auto& textureInfo : texturesInfo) {
        m_material.SetTexture(textureInfo.slotType, textureInfo.name);
    }
}
} // namespace elv
