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

    const SharedPtr<Mesh>& GetMeshPtr() const { return m_meshPtr; }
    Material& GetMaterial();

    void AddMaterialTexture(const Material::TextureSlot slot, const std::string& name, const std::string& path, bool async = true);

private:
    void LoadMesh(const RenderTopology topology);

private:
    SharedPtr<Mesh> m_meshPtr { nullptr };
    std::string m_meshName;
    std::string m_meshPath;
};
} // namespace elv
