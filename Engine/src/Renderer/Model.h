#pragma once

#if ASSIMP_MODE
#    include "Mesh.h"

#    include <assimp/material.h>

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;

namespace elv {

class Shader;
class Texture2D;

class Model {
public:
    Model(const std::string& path, bool flipUV = false)
    {
        LoadModel(path, flipUV);
    }

    void Draw(const SharedPtr<Shader>& shader) const;

private:
    void LoadModel(const std::string& path, bool flipUV);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const MeshTextureType typeName, std::vector<MeshTexture>& textures);

    void LoadMeshTexturesAsync();

private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;

    std::vector<std::string> m_texturesLoaded;
};
} // namespace elv
#endif
