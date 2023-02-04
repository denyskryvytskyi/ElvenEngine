#include "SceneManager.h"

#include "Core/SettingsConfig.h"

namespace elv {

SceneManager gSceneManager;

namespace {
} // namespace

void SceneManager::Init()
{
    m_activeScene.OnInit();

    m_serializer.Init(&m_activeScene);
    if (gEngineSettings.loadDefaultScene) {
        m_serializer.LoadScene(gEngineSettings.defaultSceneName);
    }
}

void SceneManager::Shutdown()
{
    m_serializer.SaveScene(gEngineSettings.defaultSceneName);
    if (gEngineSettings.loadDefaultScene) {
    }
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
void SceneManager::LoadScene(std::string_view scene_name)
{
    m_serializer.LoadScene(scene_name);
}

void SceneManager::SaveScene(std::string_view scene_name)
{
    m_serializer.SaveScene(scene_name);
}
} // namespace elv
