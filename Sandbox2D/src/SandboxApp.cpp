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

class TestBehavior : public Elven::ecs::IBehavior {
    void OnUpdate(float dt) override
    {
        EL_CORE_INFO("Behavior component update here...");
    }
};

namespace {
constexpr bool enableTestComponents = false;
}

Sandbox2D::Sandbox2D()
    : m_textureLoadedCallback([this](const Elven::events::TextureLoadedEvent& e) { OnTextureLoaded(e); })
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

void Sandbox2D::OnCreate()
{
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

        std::shared_ptr<Elven::Texture2D> texture = nullptr;
        texture = Elven::textures::Get("wizard_fire");

        for (size_t i = 0; i < 5; ++i) {
            for (size_t j = 0; j < 5; ++j) {
                const Elven::ecs::Entity entityQuad = scene.CreateEntity();

                scene.AddComponent<Elven::TransformComponent>(entityQuad);
                auto& q2 = scene.GetComponent<Elven::TransformComponent>(entityQuad);
                q2.pos = { static_cast<float>(i) * 0.3f, static_cast<float>(j) * 0.3f, 0.0f };
                q2.scale = { 0.25f, 0.225f };

                scene.AddComponent<Elven::SpriteComponent>(entityQuad);
                auto& sprite = scene.GetComponent<Elven::SpriteComponent>(entityQuad);
                sprite.textureName = "wizard_fire";
                sprite.texture = texture;

                if (false && j == 0 && i == 0) {
                    scene.AddComponent<Elven::BehaviorComponent>(entityQuad);
                    auto& behavior = scene.GetComponent<Elven::BehaviorComponent>(entityQuad);
                    behavior.AddBehavior<TestBehavior>();
                }
            }
        }
    }
}
