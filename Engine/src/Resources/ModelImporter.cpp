#include "ModelImporter.h"

#if ASSIMP_MODE

#    include "Core/Profiler.h"

#    include <assimp/Importer.hpp>
#    include <assimp/postprocess.h>
#    include <assimp/scene.h>

namespace elv {

void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const Material::TextureSlot textureSlotType, std::vector<MeshTexture>& textures)
{
    for (std::uint32_t i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString textureFile;
        mat->GetTexture(type, i, &textureFile);

        MeshTexture texture;
        texture.slotType = textureSlotType;
        texture.name = textureFile.C_Str();

        textures.emplace_back(std::move(texture));
    }
}

void ProcessMesh(aiMesh* mesh, const aiScene* scene, LoadedMeshInfo& info)
{
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

        info.vertices.emplace_back(vertex);
    }

    // process indices
    for (size_t i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; ++j) {
            info.indices.emplace_back(face.mIndices[j]);
        }
    }

    // process textures
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        LoadMaterialTextures(material, aiTextureType_DIFFUSE, Material::TextureSlot::Diffuse, info.textures);
        LoadMaterialTextures(material, aiTextureType_SPECULAR, Material::TextureSlot::Specular, info.textures);
        LoadMaterialTextures(material, aiTextureType_EMISSIVE, Material::TextureSlot::Emission, info.textures);
        LoadMaterialTextures(material, aiTextureType_NORMALS, Material::TextureSlot::Normal, info.textures);
        LoadMaterialTextures(material, aiTextureType_OPACITY, Material::TextureSlot::Transparency, info.textures);
    }
}

void ProcessNode(aiNode* node, const aiScene* scene, LoadedMeshesInfo& loadedMeshesInfo)
{
    // process meshes
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        LoadedMeshInfo meshInfo;
        ProcessMesh(mesh, scene, meshInfo);
        loadedMeshesInfo.meshes.emplace_back(std::move(meshInfo));
    }

    // process children
    for (size_t i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene, loadedMeshesInfo);
    }
}

void ImportModel(const std::string& path, LoadedMeshesInfo& info)
{
    EL_CORE_INFO("Model {} loading...", path);
    PROFILE(fmt::format("Model {} is loaded: ", path));

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        EL_CORE_ERROR("ASSIMP: Failed to load model {}", path);
        return;
    }

    {
        PROFILE_SCOPE(fmt::format("Process model {} nodes: ", path));
        ProcessNode(scene->mRootNode, scene, info);
    }
}

} // namespace elv

#endif
