#include "StaticMeshComponent.h"

#include "Core/FileSystem.h"
#include "Renderer/Mesh.h"
#include "Resources/MeshLibrary.h"

// async
#include "Core/StringId.h"
#include "Events/EventManager.h"
#include "Events/MeshEvent.h"

namespace elv {
void StaticMeshComponent::LoadMesh(const RenderTopology topology)
{
    if (!m_meshName.empty()) {
        auto meshPtr = gMeshLibrary.GetMesh(m_meshName);
        if (meshPtr) {
            // make new instance of cube mesh to be able to change material only for this entity
            m_meshPtr = MakeSharedPtr<Mesh>(*meshPtr);
            m_meshPtr->SetTopology(topology);
        } else if (m_meshPath.empty()) {
            EL_CORE_WARN("Failed to set mesh to the Static Mesh Component, mesh path is empty");
        } else {
            m_meshPtr = MakeSharedPtr<Mesh>();
            m_meshPtr->SetTopology(topology);

            gMeshLibrary.LoadMesh(m_meshName, m_meshPath, m_meshPtr);

            events::Subscribe<events::MeshLoadedEvent>([&](const events::MeshLoadedEvent& e) {
                if (e.meshName == m_meshName) {
                    m_meshPtr = gMeshLibrary.GetMesh(m_meshName);

                    // load textures async
                    const std::string directory = m_meshPath.substr(0, m_meshPath.find_last_of('/'));
                    if (m_meshPtr) {
                        m_meshPtr->LoadTextures(directory, true);
                    }
                }
            },
                                                       string_id(m_meshName), true);
        }
    }
}

void StaticMeshComponent::ResetMesh(const std::string& newMeshName, const RenderTopology topology)
{
    if (!newMeshName.empty()) {
        auto meshPtr = gMeshLibrary.GetMesh(newMeshName);
        if (meshPtr) {
            m_meshPtr = MakeSharedPtr<Mesh>(*meshPtr);
            m_meshPtr->SetTopology(topology);
        } else {
            EL_CORE_WARN("Failed to reset mesh: mesh {} isn't imported", newMeshName.c_str());
        }
    }
}

void StaticMeshComponent::SetTopology(const RenderTopology topology)
{
    if (m_meshPtr) {
        m_meshPtr->SetTopology(topology);
    }
}

Material* StaticMeshComponent::GetMaterial()
{
    if (m_meshPtr) {
        return &m_meshPtr->GetMaterial();
    }

    EL_CORE_ERROR("Failed to get material: mesh is empty");

    return nullptr;
}

RenderTopology StaticMeshComponent::GetRenderTopology() const
{
    if (m_meshPtr) {
        return m_meshPtr->GetRenderTopology();
    }

    return RenderTopology::Triangles;
}

void StaticMeshComponent::SetMaterialTexture(const Material::TextureSlot slot, const std::string& name, const std::string& path, bool async)
{
    m_meshPtr->SetMaterialTexture(slot, name, path, async);
}
} // namespace elv
