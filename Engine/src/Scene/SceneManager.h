#pragma once

#include "Scene/Scene.h"
#include "SceneSerializer.h"

namespace elv {
class SceneManager {
public:
    SceneManager() = default;
    SceneManager(const SceneManager&) = delete;
    const SceneManager& operator=(const SceneManager&) = delete;

    void Init();
    void Shutdown();
    void Update(float dt);
    void Render(float dt);

    void LoadScene(std::string_view sceneName);
    void SaveScene(std::string_view sceneName);

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

inline void LoadScene(std::string_view sceneName)
{
    gSceneManager.LoadScene(sceneName);
}

inline void SaveScene(std::string_view sceneName)
{
    gSceneManager.SaveScene(sceneName);
}

} // namespace elv
