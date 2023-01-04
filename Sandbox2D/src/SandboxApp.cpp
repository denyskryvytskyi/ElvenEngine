#include <Elven.h>

#include "SandboxApp.h"

#include <Core/EntryPoint.h>
#include <Events/EventManager.h>
#include <Events/TextureEvent.h>
#include <Scene/Behavior.h>
#include <Scene/Components/SceneComponents.h>
#include <Scene/SceneManager.h>

Elven::Application* Elven::CreateApplication()
{
    return new Sandbox2D();
}

// TODO: Something like RegisterBehavior to enable selection this behavior in editor
class TestBehavior : public Elven::ecs::IBehavior {
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

namespace {
constexpr bool enableTestComponents = false;
}

Sandbox2D::Sandbox2D()
    : m_textureLoadedCallback([this](const Elven::events::TextureLoadedEvent& e) { OnTextureLoaded(e); })
{
}

void Sandbox2D::OnCreate()
{
    if (enableTestComponents) {
        Elven::events::Subscribe<Elven::events::TextureLoadedEvent>(m_textureLoadedCallback, Elven::string_id("wizard"));
        Elven::events::Subscribe<Elven::events::TextureLoadedEvent>(m_textureLoadedCallback, Elven::string_id("wizard_fire"));
        Elven::events::Subscribe<Elven::events::TextureLoadedEvent>(m_textureLoadedCallback, Elven::string_id("wizard_ice"));

        std::vector<std::pair<std::string, std::string>> texturesLoadList = {
            { "wizard", "wizard.png" },
            { "wizard_fire", "wizard_fire.png" },
            { "wizard_ice", "wizard_ice.png" }
        };

        for (size_t i = 0; i < texturesLoadList.size(); i++) {
            Elven::textures::Load(texturesLoadList[i].first, texturesLoadList[i].second);
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

void Sandbox2D::OnTextureLoaded(const Elven::events::TextureLoadedEvent& e)
{
    if (e.textureName == "wizard_fire") {

        Elven::Scene& scene = Elven::GetScene();

        const Elven::SharedPtr<Elven::Texture2D> texture = Elven::textures::Get("wizard_fire");

        for (size_t i = 0; i < 5; ++i) {
            for (size_t j = 0; j < 5; ++j) {
                const Elven::ecs::Entity entityQuad = scene.CreateEntity();

                auto& transform = scene.AddComponent<Elven::TransformComponent>(
                    entityQuad,
                    lia::vec3(static_cast<float>(i) * 0.3f, static_cast<float>(j) * 0.3f, 0.0f),
                    lia::vec2(0.25f, 0.225f));

                auto& sprite = scene.AddComponent<Elven::SpriteComponent>(entityQuad, "wizard_fire");
                sprite.texture = texture;

                if (true && j == 0 && i == 0) {
                    scene.AddComponent<Elven::BehaviorComponent>(entityQuad).Bind<TestBehavior>();
                }
            }
        }
    }
}
