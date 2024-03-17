#include "MeshLibrary.h"

#include "Core/StringId.h"
#include "Events/EventManager.h"
#include "Events/MeshEvent.h"

#include "Renderer/Mesh.h"

namespace elv {

MeshLibrary gMeshLibrary;
std::mutex meshLoadingMutex;

// primitives
const std::uint32_t xSegmentsSphere = 64;
const std::uint32_t ySegmentsSphere = 64;
const std::uint32_t xSegmentsPlane = 16;
const std::uint32_t ySegmentsPlane = 16;
constexpr float PI = 3.14159265359f;
constexpr float TAU = 6.28318530717f;
//

static void LoadMeshFromFile(std::vector<LoadedMeshesInfo>& loadedMeshesInfo, const std::string& meshName, const std::string& meshPath, SharedPtr<Mesh> root)
{
    LoadedMeshesInfo info;
    info.name = meshName;
    info.root = root;

    ImportModel(meshPath, info);

    std::lock_guard<std::mutex> lock(meshLoadingMutex);
    loadedMeshesInfo.emplace_back(info);
}

void MeshLibrary::Init()
{
    LoadPrimitives();
}

void MeshLibrary::Update()
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

void MeshLibrary::LoadMesh(const std::string& name, const std::string& path, SharedPtr<Mesh> root)
{
    // check whether we already loaded this mesh
    auto it = m_meshes.find(name);

    if (it == m_meshes.end()) {
        // check whether this mesh is in loading state
        auto inProgressIt = m_meshLoadingInProgress.find(name);

        if (inProgressIt == m_meshLoadingInProgress.end()) {
            m_meshLoadingInProgress.insert(name);

            m_futures.emplace_back(std::async(std::launch::async, LoadMeshFromFile, std::ref(m_loadedMeshes), name, path, root));
        }
        return;
    }

    EL_CORE_INFO("Mesh {0} is already loaded.", name);
}

SharedPtr<Mesh> MeshLibrary::GetMesh(const std::string& name) const
{
    auto it = m_meshes.find(name);
    return it != m_meshes.end() ? it->second : nullptr;
}

void MeshLibrary::ProcessMeshInfo(const LoadedMeshesInfo& meshesInfo)
{
    // process mesh and submeshes
    bool rootInited = false;
    for (auto meshInfo : meshesInfo.meshes) {
        if (rootInited) {
            meshesInfo.root->AddSubmesh({ meshInfo.vertices, meshInfo.indices, meshInfo.textures });
        } else {
            meshesInfo.root->SetInfo(meshInfo.vertices, meshInfo.indices, std::move(meshInfo.textures));
            m_meshes.insert({ meshesInfo.name, meshesInfo.root });
            rootInited = true;
        }
    }
    events::TriggerEvent(events::MeshLoadedEvent { meshesInfo.name }, string_id(meshesInfo.name));
}

void MeshLibrary::LoadPrimitives()
{
    // ====== CUBE ======
    {
        // clang-format off
        std::vector<MeshVertex> vertices = {
            // front          // normals         // uv
            {{-0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f}, {  0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f  }, { 0.0f, 0.0f, 1.0f}, {  1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f,  }, { 0.0f, 0.0f, 1.0f}, {  1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.5f  }, { 0.0f, 0.0f, 1.0f}, {  0.0f, 1.0f}},
            // top            //  normals        // uv
            {{-0.5f, 0.5f, 0.5f  }, { 0.0f, 1.0f, 0.0f}, {  0.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f   }, { 0.0f, 1.0f, 0.0f}, {  1.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f  }, { 0.0f, 1.0f, 0.0f}, {  1.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f}, {  0.0f, 1.0f}},
            // back           //  normals         // uv
            {{0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f}, { 0.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, { 0.0f, 0.0f, -1.0f}, { 1.0f, 0.0f}},
            {{-0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f}, { 1.0f, 1.0f}},
            {{0.5f, 0.5f, -0.5f  }, { 0.0f, 0.0f, -1.0f}, { 0.0f, 1.0f}},
            // bottom         //  normals         // uv
            {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f, 0.0f}, { 0.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f}, { 1.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f  }, { 0.0f, -1.0f, 0.0f}, { 1.0f, 1.0f}},
            {{-0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f}, { 0.0f, 1.0f}},
            // left           //  normals         // uv
            {{-0.5f, -0.5f, -0.5f}, { -1.0f, 0.0f, 0.0f}, { 0.0f, 0.0f}},
            {{-0.5f, -0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f}, { 1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.5f  }, { -1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f}},
            {{-0.5f, 0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f}, { 0.0f, 1.0f}},
            // right          //  normals         // uv
            {{0.5f, -0.5f, 0.5f  }, { 1.0f, 0.0f, 0.0f}, { 0.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f}, { 1.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f  }, { 1.0f, 0.0f, 0.0f}, { 1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.5f   }, { 1.0f, 0.0f, 0.0f}, { 0.0f, 1.0f}}
        };

        std::vector<std::uint32_t> indices = {
            // front
            0,  1,  2,
            2,  3,  0,
            // top
            4,  5,  6,
            6,  7,  4,
            // back
            8,  9, 10,
            10, 11,  8,
            // bottom
            12, 13, 14,
            14, 15, 12,
            // left
            16, 17, 18,
            18, 19, 16,
            // right
            20, 21, 22,
            22, 23, 20,
        };
        // clang-format on

        m_meshes.insert({ "cube", MakeSharedPtr<Mesh>(vertices, indices) });
    }

    // ====== SPHERE ======
    {
        std::vector<MeshVertex> vertices;
        for (std::uint32_t y = 0; y <= ySegmentsSphere; ++y) {
            for (std::uint32_t x = 0; x <= xSegmentsSphere; ++x) {
                float xSegment = static_cast<float>(x) / static_cast<float>(xSegmentsSphere);
                float ySegment = static_cast<float>(y) / static_cast<float>(ySegmentsSphere);
                float xPos = std::cos(xSegment * TAU) * std::sin(ySegment * PI); // TAU is 2PI
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * TAU) * std::sin(ySegment * PI);

                MeshVertex vertex;
                vertex.Position = { xPos, yPos, zPos };
                vertex.Normal = { xPos, yPos, zPos };
                vertex.UV = { xSegment, ySegment };
                vertices.emplace_back(vertex);
            }
        }

        std::vector<std::uint32_t> indices;
        for (int y = 0; y < ySegmentsSphere; ++y) {
            for (int x = 0; x < xSegmentsSphere; ++x) {
                indices.emplace_back((y + 1) * (xSegmentsSphere + 1) + x);
                indices.emplace_back(y * (xSegmentsSphere + 1) + x);
                indices.emplace_back(y * (xSegmentsSphere + 1) + x + 1);

                indices.emplace_back((y + 1) * (xSegmentsSphere + 1) + x);
                indices.emplace_back(y * (xSegmentsSphere + 1) + x + 1);
                indices.emplace_back((y + 1) * (xSegmentsSphere + 1) + x + 1);
            }
        }

        m_meshes.insert({ "sphere", MakeSharedPtr<Mesh>(vertices, indices) });
    }

    // ====== PLANE ======
    {
        std::vector<MeshVertex> vertices;
        std::vector<std::uint32_t> indices;

        bool oddRow = false;
        float dX = 1.0f / xSegmentsPlane;
        float dY = 1.0f / ySegmentsPlane;

        for (int y = 0; y <= ySegmentsPlane; ++y) {
            for (int x = 0; x <= xSegmentsPlane; ++x) {
                MeshVertex vertex;
                vertex.Position = lia::vec3(dX * x * 2.0f - 1.0f, dY * y * 2.0f - 1.0f, 0.0f);
                vertex.UV = lia::vec2(dX * x, 1.0f - y * dY);
                vertex.Normal = lia::vec3(0.0f, 0.0f, 1.0f);
                vertices.emplace_back(vertex);
            }
        }

        for (int y = 0; y < ySegmentsPlane; ++y) {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (int x = 0; x <= xSegmentsPlane; ++x) {
                    indices.emplace_back(y * (xSegmentsPlane + 1) + x);
                    indices.emplace_back((y + 1) * (xSegmentsPlane + 1) + x);
                }
            } else {
                for (int x = xSegmentsPlane; x >= 0; --x) {
                    indices.emplace_back((y + 1) * (xSegmentsPlane + 1) + x);
                    indices.emplace_back(y * (xSegmentsPlane + 1) + x);
                }
            }
            oddRow = !oddRow;
        }

        m_meshes.insert({ "plane", MakeSharedPtr<Mesh>(vertices, indices) });
    }
}
} // namespace elv
