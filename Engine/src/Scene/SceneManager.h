#pragma once
#include "Scene/Scene.h"

namespace Elven {
class SceneManager {
public:
    SceneManager() = default;

    void Init();
    void Shutdown();
    void Update(float dt);
    void Render(float dt);

    Scene& GetScene() { return m_activeScene; }

private:
    Scene m_activeScene;
};

extern SceneManager gSceneManager;

inline Scene& GetScene()
{
    return gSceneManager.GetScene();
}

} // namespace Elven
