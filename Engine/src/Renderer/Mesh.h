#pragma once

namespace elv {

class Shader;
class Texture2D;
class VertexArray;

struct MeshVertex {
    lia::vec3 Position;
    lia::vec3 Normal;
    lia::vec2 UV;
};

enum class MeshTextureType {
    Diffuse,
    Specular,
    Emission,
    Opacity
};

struct MeshTexture {
    MeshTextureType type;
    std::string textureName;
    SharedPtr<Texture2D> texturePtr;

    void LoadTexture(const std::string& textureDir);
};

class Mesh {
public:
    Mesh();
    Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices);
    Mesh(const std::vector<MeshVertex>& vertices, const std::vector<std::uint32_t>& indices, std::vector<MeshTexture>&& textures);

    virtual ~Mesh() = default;

    void Draw(const SharedPtr<elv::Shader>& shader) const;

    void SetTextures(std::vector<MeshTexture>&& textures);

    void LoadTexturesAsync(const std::string& dir);

protected:
    void SetupMesh();

protected:
    std::vector<MeshVertex> m_vertices;
    std::vector<std::uint32_t> m_indices;
    std::vector<MeshTexture> m_textures;

private:
    SharedPtr<VertexArray> m_vao { nullptr };
};

} // namespace elv
