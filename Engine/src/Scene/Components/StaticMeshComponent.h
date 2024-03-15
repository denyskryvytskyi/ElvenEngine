#pragma once

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
    StaticMeshComponent(const std::string& name, const std::string& filePath)
        : m_meshName(name)
        , m_meshPath(filePath)
    {
        LoadMesh();
    }

    // Load primitive mesh by name: cube, sphere, conus, teapot, torus
    StaticMeshComponent(const std::string& primitiveName)
        : m_meshName(primitiveName)
    {

        LoadMesh();
    }

    const SharedPtr<Mesh>& GetMeshPtr() const { return m_meshPtr; }

private:
    void LoadMesh();

private:
    SharedPtr<Mesh> m_meshPtr { nullptr };
    std::string m_meshName;
    std::string m_meshPath;
};
} // namespace elv
