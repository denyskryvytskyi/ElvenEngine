#include <Elven.h>

#include "SandboxApp.h"

#include <Core/SettingsConfig.h>
#include <Events/TextureEvent.h>
#include <Resources/AudioManager.h>

elv::Application* elv::CreateApplication()
{
    return new Sandbox2D();
}

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
        // EL_INFO("OnUpdate of Behavior component...");
    }

    void OnRender(float dt) override
    {
        // EL_INFO("OnRender of Behavior component...");
    }
};

Sandbox2D::Sandbox2D()
    : m_textureLoadedCallback([this](const elv::events::TextureLoadedEvent& e) { OnTextureLoaded(e); })
    , m_cameraController(static_cast<float>(elv::gEngineSettings.windowWidth) / static_cast<float>(elv::gEngineSettings.windowHeight))
{
}

void Sandbox2D::OnCreate()
{
    elv::gAudioManager.AddSound("back", "back.ogg");
    elv::gAudioManager.SetVolume("back", 0.8f);
    elv::gAudioManager.Play("back");

    const bool wizardsScene = true;

    if (wizardsScene && !elv::gEngineSettings.loadDefaultScene) {
        elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, elv::string_id("wizard"));
        elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, elv::string_id("wizard_fire"));
        elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, elv::string_id("wizard_ice"));

        std::vector<std::pair<std::string, std::string>> texturesLoadList = {
            { "wizard", "wizard.png" },
            { "wizard_fire", "wizard_fire.png" },
            { "wizard_ice", "wizard_ice.png" }
        };

        for (size_t i = 0; i < texturesLoadList.size(); i++) {
            elv::textures::Load(texturesLoadList[i].first, fmt::format("{}{}", elv::fileSystem::IMAGES_PATH, texturesLoadList[i].second));
        }
        return;
    }

    // fanstasy test scene
    elv::Scene& scene = elv::GetScene();

    const elv::ecs::Entity back_entity = scene.CreateEntity();
    auto& back_sprite = scene.AddComponent<elv::SpriteComponent>(back_entity);
    back_sprite.SetTexture("battleground", "Battleground.png");
    auto& back_transform = scene.AddComponent<elv::TransformComponent>(back_entity);
    back_transform.scale = { 384.0f, 216.0f, 1.0f };

    const elv::ecs::Entity skeleton_entity = scene.CreateEntity();
    auto& skeleton_sprite = scene.AddComponent<elv::SpriteComponent>(skeleton_entity);
    skeleton_sprite.SetTexture("skeleton", "Attack_3.png");
    auto& skeleton_transform = scene.AddComponent<elv::TransformComponent>(skeleton_entity);
    skeleton_transform.scale = { 400.0f, 100.0f, 1.0f };
    skeleton_transform.pos = { 50.0f, -25.0f, 0.0f };

    const elv::ecs::Entity magic_entity = scene.CreateEntity();
    auto& magic_sprite = scene.AddComponent<elv::SpriteComponent>(magic_entity);
    magic_sprite.SetTexture("wizard", "Attack_1.png");
    auto& magic_transform = scene.AddComponent<elv::TransformComponent>(magic_entity);
    magic_transform.scale = { 700.0f, 100.0f, 1.0f };
    magic_transform.pos = { -50.0f, -25.0f, 0.0f };
}

void Sandbox2D::OnUpdate(float dt)
{
    m_cameraController.OnUpdate(dt);
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

        const elv::SharedPtr<elv::Texture> texture = elv::textures::Get("wizard_fire");

        for (size_t i = 0; i < 100; ++i) {
            for (size_t j = 0; j < 100; ++j) {
                const elv::ecs::Entity entityQuad = scene.CreateEntity();

                auto& transform = scene.AddComponent<elv::TransformComponent>(
                    entityQuad,
                    lia::vec3(static_cast<float>(i) * 13.0f, static_cast<float>(j) * 13.0f, 0.0f),
                    lia::vec3(10.0f, 10.0f, 1.0f));

                auto& sprite = scene.AddComponent<elv::SpriteComponent>(entityQuad, "wizard_fire", "wizard_fire.png");
                sprite.texture = texture;

                if (true && j == 0 && i == 0) {
                    scene.AddComponent<elv::BehaviorComponent>(entityQuad).Bind<TestBehavior>();
                }
            }
        }
    }
}
