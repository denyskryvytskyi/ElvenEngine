#pragma once

#include "Renderer/Material.h"
#include "Renderer/RenderTopology.h"

namespace elv {
class Mesh;

class StaticMeshComponent {
public:
    StaticMeshComponent() = default;

    /**
     * Set and load static mesh
     *
     * @param name Static mesh name to store in mesh library
     * @param filePath Relative path to the file
     */
    StaticMeshComponent(const std::string& name, const std::string& filePath, const RenderTopology topology = RenderTopology::Triangles)
        : m_meshName(name)
        , m_meshPath(filePath)
    {
        LoadMesh(topology);
    }

    // Load primitive mesh by name: cube, sphere, conus, teapot, torus
    StaticMeshComponent(const std::string& primitiveName, const RenderTopology topology = RenderTopology::Triangles)
        : m_meshName(primitiveName)
    {
        LoadMesh(topology);
    }

    void ResetMesh(const std::string& newMeshName, const RenderTopology topology = RenderTopology::Triangles);
    void SetMaterialTexture(const Material::TextureSlot slot, const std::string& name, const std::string& path, bool async = true);
    void SetTopology(const RenderTopology topology);
    void SetName(const std::string& name) { m_meshName = name; }

    const SharedPtr<Mesh>& GetMeshPtr() const { return m_meshPtr; }
    const std::string& GetMeshName() const { return m_meshName; }
    const std::string& GetMeshPath() const { return m_meshPath; }
    Material* GetMaterial();
    RenderTopology GetRenderTopology() const;

private:
    void LoadMesh(const RenderTopology topology);

private:
    SharedPtr<Mesh> m_meshPtr { nullptr };
    std::string m_meshName;
    std::string m_meshPath;
};
} // namespace elv
