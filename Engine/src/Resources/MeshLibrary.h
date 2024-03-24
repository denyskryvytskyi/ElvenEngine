#pragma once

#include "ModelImporter.h"

namespace elv {
class Mesh;

class MeshLibrary {
public:
    MeshLibrary() = default;
    MeshLibrary(const MeshLibrary&) = delete;
    const MeshLibrary& operator=(const MeshLibrary&) = delete;

    void Init();
    void Update();
    void Shutdown();

    void LoadMesh(const std::string& name, const std::string& path, SharedPtr<Mesh> root);

    SharedPtr<Mesh> GetMesh(const std::string& name) const;

    // return list of names of all meshes
    std::vector<std::string> GetMeshes() const;

private:
    void ProcessMeshInfo(const LoadedMeshesInfo& meshInfo);
    void LoadPrimitives();

private:
    std::unordered_map<std::string, SharedPtr<Mesh>> m_meshes;

    std::set<std::string> m_meshLoadingInProgress;
    std::vector<LoadedMeshesInfo> m_loadedMeshes;
    std::vector<std::future<void>> m_futures;
};

extern MeshLibrary gMeshLibrary;

} // namespace elv
