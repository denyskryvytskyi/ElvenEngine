#pragma once

#include "Material.h"

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
    Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices);
    Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices, const std::vector<MeshTexture>& texturesInfo);

    virtual ~Mesh() = default;

    void Draw(const SharedPtr<elv::Shader>& shader) const;

    void LoadTextures(const std::string& dir, const bool async);

    void AddSubmesh(Mesh&& submesh);

    Material& GetMaterial() { return m_material; }

protected:
    void SetupMesh();

private:
    void SetupMaterial(const std::vector<MeshTexture>& texturesInfo);

protected:
    std::vector<MeshVertex> m_vertices;
    std::vector<std::uint32_t> m_indices;
    Material m_material;

private:
    SharedPtr<VertexArray> m_vao { nullptr };
    std::vector<Mesh> m_submeshes;
};

} // namespace elv
