#include "SceneManager.h"
#include "Scene.h"

namespace Elven {

SceneManager gSceneManager;

void SceneManager::Init()
{
    m_activeScene.OnInit();
}

void SceneManager::Shutdown()
{
    m_activeScene.OnShutdown();
}

void SceneManager::Update(float dt)
{
    m_activeScene.OnUpdate(dt);
}

void SceneManager::Render(float dt)
{
    m_activeScene.OnRender(dt);
}
} // namespace Elven
