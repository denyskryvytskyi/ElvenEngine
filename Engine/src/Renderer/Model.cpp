#include "Model.h"
#if ASSIMP_MODE

#    include "Shader.h"
#    include "Texture2D.h"

#    include "Core/FileSystem.h"
#    include "Core/StringId.h"
#    include "Events/EventManager.h"
#    include "Events/TextureEvent.h"
#    include "Resources/TextureManager.h"

#    include <assimp/Importer.hpp>
#    include <assimp/postprocess.h>
#    include <assimp/scene.h>

namespace elv {

void Model::Draw(const SharedPtr<Shader>& shader) const
{
    for (auto& mesh : m_meshes)
        mesh.Draw(shader);
}

void Model::LoadModel(const std::string& path, bool flipUV = false)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        EL_CORE_ERROR("ASSIMP: Failed to load model {}", path);
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);

    LoadMeshTexturesAsync();
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process meshes
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.emplace_back(ProcessMesh(mesh, scene));
    }

    // process children
    for (size_t i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<MeshVertex> vertices;
    std::vector<std::uint32_t> indices;
    std::vector<MeshTexture> textures;

    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
        // process vertix position, normal and UV
        MeshVertex vertex;
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;

        if (mesh->mNormals) {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
        }

        // if mesh has uv
        if (mesh->mTextureCoords[0]) {
            vertex.UV.x = mesh->mTextureCoords[0][i].x;
            vertex.UV.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.emplace_back(vertex);
    }

    // process indices
    for (size_t i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; ++j) {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    // process textures
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        LoadMaterialTextures(material, aiTextureType_DIFFUSE, MeshTextureType::Diffuse, textures);
        LoadMaterialTextures(material, aiTextureType_SPECULAR, MeshTextureType::Specular, textures);
        LoadMaterialTextures(material, aiTextureType_EMISSIVE, MeshTextureType::Emission, textures);
        LoadMaterialTextures(material, aiTextureType_OPACITY, MeshTextureType::Opacity, textures);
    }

    return Mesh(vertices, indices, std::move(textures));
}

void Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const MeshTextureType typeName, std::vector<MeshTexture>& textures)
{
    for (size_t i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString textureFile;
        mat->GetTexture(type, i, &textureFile);

        MeshTexture texture;
        texture.type = typeName;
        texture.textureName = textureFile.C_Str();

        textures.emplace_back(std::move(texture));
    }
}

void Model::LoadMeshTexturesAsync()
{
    for (auto& mesh : m_meshes) {
        mesh.LoadTexturesAsync(m_directory);
    }
}

} // namespace elv

#endif ASSIMP_MODE
