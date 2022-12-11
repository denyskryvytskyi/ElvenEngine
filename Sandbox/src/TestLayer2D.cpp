#include "TestLayer2D.h"
#include "Elven.h"

#include <Events/EventManager.h>
#include <Events/TextureEvent.h>
#include <Scene/Behavior.h>
#include <Scene/Components/SceneComponents.h>
#include <Scene/SceneManager.h>

class TestBehavior : public Elven::ecs::IBehavior {
    void OnUpdate(float dt) override
    {
        EL_CORE_INFO("Behavior component update here...");
    }
};

TestLayer2D::TestLayer2D()
    : Layer("TestLayer2D")
    , m_textureLoadedCallback([this](const Elven::events::TextureLoadedEvent& e) { OnTextureLoaded(e); })
{
    Elven::events::Subscribe<Elven::events::TextureLoadedEvent>(m_textureLoadedCallback);

    std::vector<std::pair<std::string, std::string>> texturesLoadList = {
        { "wizard", "wizard.png" },
        { "wizard_fire", "wizard_fire.png" },
        { "wizard_ice", "wizard_ice.png" }
    };

    for (size_t i = 0; i < texturesLoadList.size(); i++) {
        Elven::textures::Load(std::move(texturesLoadList[i].first), texturesLoadList[i].second);
    }
}

TestLayer2D::~TestLayer2D()
{
}

void TestLayer2D::OnAttach()
{
}

void TestLayer2D::OnDetach()
{
}

void TestLayer2D::OnUpdate(float dt)
{
}

void TestLayer2D::OnImGuiRender()
{
}

void TestLayer2D::OnTextureLoaded(const Elven::events::TextureLoadedEvent& e)
{
    if (e.textureName == "wizard_fire") {

        Elven::Scene& scene = Elven::GetScene();

        std::shared_ptr<Elven::Texture2D> texture = nullptr;
        texture = Elven::textures::Get("wizard_fire");

        for (size_t i = 0; i < 100; ++i) {
            for (size_t j = 0; j < 200; ++j) {
                const Elven::ecs::Entity entityQuad = scene.CreateEntity();

                scene.AddComponent<Elven::TransformComponent>(entityQuad);
                auto& q2 = scene.GetComponent<Elven::TransformComponent>(entityQuad);
                q2.pos = { static_cast<float>(i) * 0.3f, static_cast<float>(j) * 0.3f, 0.0f };
                q2.scale = { 0.25f, 0.225f };

                scene.AddComponent<Elven::SpriteComponent>(entityQuad);
                auto& sprite = scene.GetComponent<Elven::SpriteComponent>(entityQuad);
                sprite.m_texture = texture;

                if (j == 0 && i == 0) {
                    scene.AddComponent<Elven::BehaviorComponent>(entityQuad);
                    auto& behavior = scene.GetComponent<Elven::BehaviorComponent>(entityQuad);
                    behavior.AddBehavior<TestBehavior>();
                }
            }
        }
    }
}
