#include <Elven.h>

#include "SandboxApp.h"

#include <Core/EntryPoint.h>
#include <Core/SettingsConfig.h>
#include <Events/EventManager.h>
#include <Events/TextureEvent.h>
#include <Scene/Behavior.h>
#include <Scene/Components/SceneComponents.h>
#include <Scene/SceneManager.h>

elv::Application* elv::CreateApplication()
{
    return new Sandbox2D();
}

// TODO: Something like RegisterBehavior to enable selection this behavior in editor
class TestBehavior : public elv::ecs::IBehavior {
public:
    void OnCreate() override
    {
        EL_INFO("OnCreate of Behavior component...");
    }

    void OnDestroy() override
    {
        EL_INFO("OnDestroy of Behavior component...");
    }

    void OnUpdate(float dt) override
    {
        EL_INFO("OnUpdate of Behavior component...");
    }

    void OnRender(float dt) override
    {
        EL_INFO("OnRender of Behavior component...");
    }
};

Sandbox2D::Sandbox2D()
    : m_textureLoadedCallback([this](const elv::events::TextureLoadedEvent& e) { OnTextureLoaded(e); })
{
}

void Sandbox2D::OnCreate()
{
    if (!elv::gEngineSettings.LoadDefaultScene) {
        elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, elv::string_id("wizard"));
        elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, elv::string_id("wizard_fire"));
        elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, elv::string_id("wizard_ice"));

        std::vector<std::pair<std::string, std::string>> texturesLoadList = {
            { "wizard", "wizard.png" },
            { "wizard_fire", "wizard_fire.png" },
            { "wizard_ice", "wizard_ice.png" }
        };

        for (size_t i = 0; i < texturesLoadList.size(); i++) {
            elv::textures::Load(texturesLoadList[i].first, texturesLoadList[i].second);
        }
    }
}

void Sandbox2D::OnUpdate(float dt)
{
}

void Sandbox2D::OnRender(float dt)
{
}

void Sandbox2D::OnDestroy()
{
}

void Sandbox2D::OnTextureLoaded(const elv::events::TextureLoadedEvent& e)
{
    if (e.textureName == "wizard_fire") {

        elv::Scene& scene = elv::GetScene();

        const elv::SharedPtr<elv::Texture2D> texture = elv::textures::Get("wizard_fire");

        for (size_t i = 0; i < 25; ++i) {
            for (size_t j = 0; j < 25; ++j) {
                const elv::ecs::Entity entityQuad = scene.CreateEntity();

                auto& transform = scene.AddComponent<elv::TransformComponent>(
                    entityQuad,
                    lia::vec3(static_cast<float>(i) * 0.3f, static_cast<float>(j) * 0.3f, 0.0f),
                    lia::vec2(0.25f, 0.225f));

                auto& sprite = scene.AddComponent<elv::SpriteComponent>(entityQuad, "wizard_fire");
                sprite.texture = texture;

                if (true && j == 0 && i == 0) {
                    scene.AddComponent<elv::BehaviorComponent>(entityQuad).Bind<TestBehavior>();
                }
            }
        }
    }
}
