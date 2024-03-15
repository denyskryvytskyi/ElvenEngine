#include "MeshLibrary.h"

#include "Core/StringId.h"
#include "Events/EventManager.h"
#include "Events/MeshEvent.h"

#include "Renderer/Mesh.h"

namespace elv {
MeshLibrary gMeshLibrary;

std::mutex meshLoadingMutex;

static void LoadMeshFromFile(std::vector<LoadedMeshesInfo>& loadedMeshesInfo, const std::string& meshName, const std::string& meshPath)
{
    LoadedMeshesInfo info;
    info.name = meshName;
    info.texturesDir = meshPath;

    ImportModel(meshPath, info);

    std::lock_guard<std::mutex> lock(meshLoadingMutex);
    loadedMeshesInfo.emplace_back(info);
}

MeshLibrary::MeshLibrary()
{
    LoadPrimitives();
}

void MeshLibrary::LoadMesh(const std::string& name, const std::string& path)
{
    // check whether we already loaded this mesh
    auto it = m_meshes.find(name);

    if (it == m_meshes.end()) {
        // check whether this mesh is in loading state
        auto inProgressIt = m_meshLoadingInProgress.find(name);

        if (inProgressIt == m_meshLoadingInProgress.end()) {
            m_meshLoadingInProgress.insert(name);

            m_futures.emplace_back(std::async(std::launch::async, LoadMeshFromFile, std::ref(m_loadedMeshes), name, path));
        }
        return;
    }

    EL_CORE_INFO("Mesh {0} is already loaded.", name);
}

void MeshLibrary::OnUpdate()
{
    if (meshLoadingMutex.try_lock()) {
        for (const auto& meshInfo : m_loadedMeshes) {
            ProcessMeshInfo(meshInfo);
        }
        m_loadedMeshes.clear();
        meshLoadingMutex.unlock();
    }
}

void MeshLibrary::Shutdown()
{
    for (const auto& futureTask : m_futures) {
        futureTask.wait();
    }

    m_futures.clear();
}

SharedPtr<Mesh> MeshLibrary::GetMesh(const std::string& name) const
{
    auto it = m_meshes.find(name);
    return it != m_meshes.end() ? it->second : nullptr;
}

void MeshLibrary::ProcessMeshInfo(const LoadedMeshesInfo& meshesInfo)
{
    // process mesh and submeshes
    SharedPtr<Mesh> root = nullptr;
    for (auto meshInfo : meshesInfo.meshes) {
        if (root) {
            root->AddSubmesh({ meshInfo.vertices, meshInfo.indices, meshInfo.textures });
        } else {
            root = MakeSharedPtr<Mesh>(meshInfo.vertices, meshInfo.indices, std::move(meshInfo.textures));
            m_meshes.insert({ meshesInfo.name, root });
        }
    }
    events::TriggerEvent(events::MeshLoadedEvent { meshesInfo.name }, string_id(meshesInfo.name));
}

void MeshLibrary::LoadPrimitives()
{
    // cube
}
} // namespace elv
