#pragma once

#include "Material.h"
#include "RenderTopology.h"

namespace elv {

class Shader;
class Texture2D;
class VertexArray;

struct MeshVertex {
    lia::vec3 Position;
    lia::vec3 Normal;
    lia::vec2 UV;
};

struct MeshTexture {
    Material::TextureSlot slotType;
    std::string name;
};

class Mesh {
public:
    Mesh();
    Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices, const RenderTopology topology = RenderTopology::Triangles);
    Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices, const std::vector<MeshTexture>& texturesInfo);

    virtual ~Mesh() = default;

    void SetInfo(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices, const std::vector<MeshTexture>& texturesInfo);
    void Draw(const SharedPtr<elv::Shader>& shader) const;
    void AddSubmesh(Mesh&& submesh);

    void LoadTextures(const std::string& dir, const bool async);

    void SetTopology(const RenderTopology topology) { m_topology = topology; }

    Material& GetMaterial() { return m_material; }

private:
    void SetupMesh();
    void SetupMaterial(const std::vector<MeshTexture>& texturesInfo);

private:
    RenderTopology m_topology { RenderTopology::Triangles };
    std::vector<MeshVertex> m_vertices;
    std::vector<std::uint32_t> m_indices;
    Material m_material;

    SharedPtr<VertexArray> m_vao { nullptr };
    std::vector<Mesh> m_submeshes;
};

} // namespace elv
