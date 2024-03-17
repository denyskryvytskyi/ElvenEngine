#pragma once

#include "Renderer/Mesh.h"

namespace elv {

struct LoadedMeshInfo {
    std::vector<MeshVertex> vertices;
    std::vector<std::uint32_t> indices;
    std::vector<MeshTexture> textures;
};

struct LoadedMeshesInfo {
    std::string name;
    SharedPtr<Mesh> root;
    std::vector<LoadedMeshInfo> meshes;
};

void ImportModel(const std::string& path, LoadedMeshesInfo& info);
} // namespace elv
