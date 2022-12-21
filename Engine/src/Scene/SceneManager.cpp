#include "SceneManager.h"

#include "Core/SettingsConfig.h"

namespace Elven {

SceneManager gSceneManager;

namespace {
} // namespace

void SceneManager::Init()
{
    m_activeScene.OnInit();

    m_serializer.Init(&m_activeScene);
    m_serializer.LoadScene(gEngineSettings.DefaultSceneName);
}

void SceneManager::Shutdown()
{
    m_serializer.SaveScene("new_" + gEngineSettings.DefaultSceneName);
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
