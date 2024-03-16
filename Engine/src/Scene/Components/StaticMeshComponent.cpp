#include "StaticMeshComponent.h"

#include "Core/FileSystem.h"
#include "Renderer/Mesh.h"
#include "Resources/MeshLibrary.h"

// async
#include "Core/StringId.h"
#include "Events/EventManager.h"
#include "Events/MeshEvent.h"

namespace elv {
#pragma optimize("", off)
void StaticMeshComponent::LoadMesh(const RenderTopology topology)
{
    if (!m_meshName.empty()) {
        auto meshPtr = gMeshLibrary.GetMesh(m_meshName);
        if (meshPtr) {
            m_meshPtr = meshPtr;
            m_meshPtr->SetTopology(topology);
        } else if (m_meshPath.empty()) {
            EL_CORE_WARN("Failed to set mesh to the Static Mesh Component, mesh path is empty");
        } else {
            gMeshLibrary.LoadMesh(m_meshName, m_meshPath);
            events::Subscribe<events::MeshLoadedEvent>([&](const events::MeshLoadedEvent& e) {
                if (e.meshName == m_meshName) {
                    m_meshPtr = gMeshLibrary.GetMesh(m_meshName);

                    // load textures async
                    const std::string directory = m_meshPath.substr(0, m_meshPath.find_last_of('/'));
                    if (m_meshPtr) {
                        m_meshPtr->SetTopology(topology);
                        m_meshPtr->LoadTextures(directory, true);
                    }
                }
            },
                                                       string_id(m_meshName), true);
        }
    }
}
} // namespace elv
