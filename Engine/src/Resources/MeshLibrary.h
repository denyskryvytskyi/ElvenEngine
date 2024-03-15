#pragma once

#include "ModelImporter.h"

namespace elv {
class Mesh;

class MeshLibrary {
public:
    MeshLibrary();

    void LoadMesh(const std::string& name, const std::string& path);

    void OnUpdate();
    void Shutdown();

    SharedPtr<Mesh> GetMesh(const std::string& name) const;

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
