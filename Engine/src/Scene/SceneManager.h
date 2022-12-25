#pragma once

#include "Scene/Scene.h"
#include "SceneSerializer.h"

namespace Elven {
class SceneManager {
public:
    SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    const SceneManager& operator=(const SceneManager&) = delete;

    void Init();
    void Shutdown();
    void Update(float dt);
    void Render(float dt);

    Scene& GetScene() { return m_activeScene; }

private:
    Scene m_activeScene;
    SceneSerializer m_serializer;
};

extern SceneManager gSceneManager;

inline Scene& GetScene()
{
    return gSceneManager.GetScene();
}

} // namespace Elven
