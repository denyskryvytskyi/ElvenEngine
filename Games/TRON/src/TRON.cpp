#include <Elven.h>

#include "TRON.h"
#include <Resources/AudioManager.h>

elv::Application* elv::CreateApplication()
{
    return new TRON();
}

namespace {
constexpr float segmentSize = 1.0f;
constexpr float lightCycleVelocity = 1.0f;

const lia::vec4 lightCycleColor { 0.8f, 0.8f, 0.8f, 1.0f };

const lia::vec4 player1LineColor { 0.0f, 0.2f, 0.8f, 1.0f };
const lia::vec4 player2LineColor { 0.7f, 0.5f, 0.0f, 1.0f };

constexpr float paddleStartPosOffset = 20.0f;

std::vector<elv::ecs::Entity> m_lineSegments;
} // namespace

struct LightCycleComponent {
    lia::vec4 lineColor { player1LineColor };
    lia::vec3 startVelocity { 1.0f, 0.0f, 0.0f };
    elv::Keycode upKey { elv::key::W };
    elv::Keycode downKey { elv::key::S };
    elv::Keycode leftKey { elv::key::A };
    elv::Keycode rightKey { elv::key::D };
};

// AABB - AABB collision
static bool CheckCollision(const elv::TransformComponent& lightcycle, const elv::TransformComponent& lineSegment)
{
    return ((lightcycle.pos.x + lightcycle.scale.x * 0.5f) > (lineSegment.pos.x - lineSegment.scale.x * 0.5f)) // check right side of lightcycle >= left side of segment
        && ((lineSegment.pos.x + lineSegment.scale.x * 0.5f) > lightcycle.pos.x - lightcycle.scale.x * 0.5f)   // check right side of segment >= left side of lightcycle
        && ((lightcycle.pos.y + lightcycle.scale.y * 0.5f) > (lineSegment.pos.y - lineSegment.scale.y * 0.5f)) // check top side of lightcycle >= down side of segment
        && ((lineSegment.pos.y + lineSegment.scale.y * 0.5f) > lightcycle.pos.y - lightcycle.scale.y * 0.5f);  // check top side of segment >= down side of lightcycle
}

class LightCycleBehavior : public elv::ecs::IBehavior {
    void OnCreate() override
    {
        m_lightcycle = GetComponent<LightCycleComponent>();
        m_velocity = m_lightcycle.startVelocity;
        timer.Restart();
    }

    void OnUpdate(float dt) override
    {
        if (elv::Input::IsKeyPressed(m_lightcycle.leftKey) && m_velocity.x != lightCycleVelocity) {
            m_velocity.y = 0.0f;
            m_velocity.x = -lightCycleVelocity;
        } else if (elv::Input::IsKeyPressed(m_lightcycle.rightKey) && m_velocity.x != -lightCycleVelocity) {
            m_velocity.y = 0.0f;
            m_velocity.x = lightCycleVelocity;
        } else if (elv::Input::IsKeyPressed(m_lightcycle.downKey) && m_velocity.y != lightCycleVelocity) {
            m_velocity.y = -lightCycleVelocity;
            m_velocity.x = 0.0f;
        } else if (elv::Input::IsKeyPressed(m_lightcycle.upKey) && m_velocity.y != -lightCycleVelocity) {
            m_velocity.y = lightCycleVelocity;
            m_velocity.x = 0.0f;
        }

        if (timer.ElapsedMs() >= m_speedMs) {
            elv::TransformComponent& transform = GetComponent<elv::TransformComponent>();
            SpawnLineSegment(transform.pos);
            transform.pos += m_velocity * segmentSize;
            timer.Restart();
        }
    }

private:
    void SpawnLineSegment(const lia::vec3& lightcyclePos)
    {
        const auto entity = p_Scene->CreateEntity();

        auto& segmentTransform = p_Scene->AddComponent<elv::TransformComponent>(entity);
        segmentTransform.pos = lightcyclePos;
        segmentTransform.scale = { segmentSize, segmentSize, 1.0f };

        auto& segmentQuad = p_Scene->AddComponent<elv::QuadComponent>(entity);
        segmentQuad.color = m_lightcycle.lineColor;

        m_lineSegments.emplace_back(entity);
    }

private:
    lia::vec3 m_velocity { 1.0f, 0.0f, 0.0f };
    float m_speedMs { 16.0f }; // ms to generate next segment
    elv::Timer timer;

    LightCycleComponent m_lightcycle;
};

void TRON::OnCreate()
{
    elv::gAudioManager.AddSound("back", "back.mp3");
    elv::gAudioManager.SetVolume("back", 0.5f);
    elv::gAudioManager.Play("back", true);

    elv::Scene& scene = elv::GetScene();
    scene.RegisterComponent<LightCycleComponent>();

    // background
    auto back_entity = scene.CreateEntity();
    auto& transformComponent = scene.AddComponent<elv::TransformComponent>(back_entity);
    transformComponent.scale = { 700.0f, 700.0f, 1.0f };
    scene.AddComponent<elv::SpriteComponent>(back_entity, "background", "background.png");

    // text
    m_startMenuText1Entity = scene.CreateEntity();
    scene.AddComponent<elv::RectTransformComponent>(m_startMenuText1Entity, lia::vec2(37.0f, 30.0f), lia::vec2(0.6f, 0.6f));
    scene.AddComponent<elv::TextComponent>(m_startMenuText1Entity, "Grid initialization...");
    m_startMenuText2Entity = scene.CreateEntity();
    scene.AddComponent<elv::RectTransformComponent>(m_startMenuText2Entity, lia::vec2(37.0f, 40.0f), lia::vec2(0.6f, 0.6f));
    scene.AddComponent<elv::TextComponent>(m_startMenuText2Entity, "Press P to start the line");
    m_startMenuText3Entity = scene.CreateEntity();
    scene.AddComponent<elv::RectTransformComponent>(m_startMenuText3Entity, lia::vec2(37.0f, 50.0f), lia::vec2(0.6f, 0.6f));
    scene.AddComponent<elv::TextComponent>(m_startMenuText3Entity, "Press Q to leave the line");

    m_gameOverTextEntity = scene.CreateEntity();
    scene.AddComponent<elv::RectTransformComponent>(m_gameOverTextEntity, lia::vec2(37.0f, 30.0f), lia::vec2(0.6f, 0.6f));
    auto& gameOverText = scene.AddComponent<elv::TextComponent>(m_gameOverTextEntity);
    gameOverText.Hide();

    m_restartMenuTextEntity = scene.CreateEntity();
    scene.AddComponent<elv::RectTransformComponent>(m_restartMenuTextEntity, lia::vec2(37.0f, 40.0f), lia::vec2(0.6f, 0.6f));
    auto& restartText = scene.AddComponent<elv::TextComponent>(m_restartMenuTextEntity, "Press R to restart the line");
    restartText.Hide();

    m_pauseMenuTextEntity = scene.CreateEntity();
    scene.AddComponent<elv::RectTransformComponent>(m_pauseMenuTextEntity, lia::vec2(37.0f, 30.0f), lia::vec2(0.6f, 0.6f));
    auto& pauseText = scene.AddComponent<elv::TextComponent>(m_pauseMenuTextEntity, "Press Space to resume the line");
    pauseText.Hide();

    // event handlers
    elv::events::EventHandler<elv::events::KeyPressedEvent> keyPressedHandler([this](const elv::events::KeyPressedEvent& e) { OnKeyPressedEvent(e); });
    elv::events::Subscribe<elv::events::KeyPressedEvent>(keyPressedHandler);
}

void TRON::OnUpdate(float dt)
{
    if (m_gameState == GameState::Play) {
        OnPlay();
    }
}

void TRON::OnPlay()
{
    auto& scene = elv::GetScene();

    bool isGameOver = false;
    // check collisions
    for (size_t i = 0; i < 2; i++) {
        auto& playerTransform = scene.GetComponent<elv::TransformComponent>(m_players[i].entity);

        // check collion with boundaries
        auto cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();
        if (playerTransform.pos.x <= cameraBounds.left || playerTransform.pos.x >= cameraBounds.right
            || playerTransform.pos.y >= cameraBounds.top || playerTransform.pos.y <= cameraBounds.bottom) {
            if (isGameOver) { // draw
                m_winnerId = -1;
            } else {
                isGameOver = true;
                m_winnerId = i == 0 ? 1 : 0;
            }
            break;
        }

        // check collision with lines
        for (auto segmentEntity : m_lineSegments) {
            auto& segmentTransform = scene.GetComponent<elv::TransformComponent>(segmentEntity);
            if (CheckCollision(playerTransform, segmentTransform)) {
                if (isGameOver) { // draw
                    m_winnerId = -1;
                } else {
                    isGameOver = true;
                    m_winnerId = i == 0 ? 1 : 0;
                }
                break;
            }
        }
    }

    if (isGameOver) {
        OnGameOver();
    }
}

void TRON::OnGameOver()
{
    auto& scene = elv::GetScene();

    // remove redundant components from players
    for (auto player : m_players) {
        scene.RemoveComponent<elv::BehaviorComponent>(player.entity);
    }

    std::string result;
    if (m_winnerId != -1) {
        ++m_players[m_winnerId].score;
        scene.GetComponent<elv::TextComponent>(m_players[m_winnerId].entity).text = fmt::format("{}", m_players[m_winnerId].score);
        result = fmt::format("Player {} win", m_winnerId + 1);
    } else {
        result = "End of line...";
    }

    scene.GetComponent<elv::TextComponent>(m_gameOverTextEntity).text = std::move(result);
    scene.GetComponent<elv::TextComponent>(m_gameOverTextEntity).Show();
    scene.GetComponent<elv::TextComponent>(m_restartMenuTextEntity).Show();

    m_gameState = GameState::GameOver;
}

void TRON::OnPause()
{
    auto& scene = elv::GetScene();

    if (m_gameState == GameState::Play) {
        for (auto player : m_players) {
            scene.GetComponent<elv::BehaviorComponent>(player.entity).Disable();
        }
        m_gameState = GameState::Pause;
        scene.GetComponent<elv::TextComponent>(m_pauseMenuTextEntity).Show();
    } else if (m_gameState == GameState::Pause) {
        for (auto player : m_players) {
            scene.GetComponent<elv::BehaviorComponent>(player.entity).Enable();
        }
        m_gameState = GameState::Play;
        scene.GetComponent<elv::TextComponent>(m_pauseMenuTextEntity).Hide();
    }
}

void TRON::OnStartGame()
{
    if (m_gameState == GameState::Menu) {

        auto& scene = elv::GetScene();

        // hide start menu
        scene.DestroyEntity(m_startMenuText1Entity);
        scene.DestroyEntity(m_startMenuText2Entity);
        scene.DestroyEntity(m_startMenuText3Entity);

        auto cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();

        // player 1
        auto player1 = scene.CreateEntity();
        m_players[0].entity = player1;

        scene.AddComponent<elv::BehaviorComponent>(player1).Bind<LightCycleBehavior>();

        auto& transformPlayer1 = scene.AddComponent<elv::TransformComponent>(player1);
        transformPlayer1.pos = { cameraBounds.left + paddleStartPosOffset, 0.0f, 0.0f };
        transformPlayer1.scale = { segmentSize, segmentSize, 1.0f };

        auto& quadPlayer1 = scene.AddComponent<elv::QuadComponent>(player1);
        quadPlayer1.color = lightCycleColor;
        scene.AddComponent<LightCycleComponent>(player1);

        scene.AddComponent<elv::RectTransformComponent>(player1, lia::vec2(0.0f));
        scene.AddComponent<elv::TextComponent>(player1, "0", player1LineColor);

        // player 2
        auto player2 = scene.CreateEntity();
        m_players[1].entity = player2;

        scene.AddComponent<elv::BehaviorComponent>(player2).Bind<LightCycleBehavior>();
        auto& transformPlayer2 = scene.AddComponent<elv::TransformComponent>(player2);

        transformPlayer2.pos = { cameraBounds.right - paddleStartPosOffset, 0.0f, 0.0f };
        transformPlayer2.scale = { segmentSize, segmentSize, 1.0f };

        auto& quad_player2 = scene.AddComponent<elv::QuadComponent>(player2);
        quad_player2.color = lightCycleColor;

        auto& lightcycleComponent = scene.AddComponent<LightCycleComponent>(player2);
        lightcycleComponent.lineColor = player2LineColor;
        lightcycleComponent.startVelocity = { -1.0f, 0.0f, 0.0f };
        lightcycleComponent.upKey = elv::key::Up;
        lightcycleComponent.downKey = elv::key::Down;
        lightcycleComponent.leftKey = elv::key::Left;
        lightcycleComponent.rightKey = elv::key::Right;

        scene.AddComponent<elv::RectTransformComponent>(player2, lia::vec2(95.0f, 0.0f));
        scene.AddComponent<elv::TextComponent>(player2, "0", player2LineColor);

        //
        m_gameState = GameState::Play;
    }
}

void TRON::OnRestart()
{
    if (m_gameState == GameState::GameOver) {

        m_winnerId = -1; // reset winner

        auto& scene = elv::GetScene();

        // hide text
        scene.GetComponent<elv::TextComponent>(m_gameOverTextEntity).Hide();
        scene.GetComponent<elv::TextComponent>(m_restartMenuTextEntity).Hide();

        // destroy lines
        for (auto segmentEntity : m_lineSegments) {
            scene.DestroyEntity(segmentEntity);
        }
        m_lineSegments.clear();

        // reset players
        for (auto player : m_players) {
            scene.AddComponent<elv::BehaviorComponent>(player.entity).Bind<LightCycleBehavior>();
        }

        auto cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();

        auto& player1Transform = scene.GetComponent<elv::TransformComponent>(m_players[0].entity);
        player1Transform.pos.x = cameraBounds.left + paddleStartPosOffset;
        player1Transform.pos.y = { 0.0f };

        auto& player2Transform = scene.GetComponent<elv::TransformComponent>(m_players[1].entity);
        player2Transform.pos.x = cameraBounds.right - paddleStartPosOffset;
        player2Transform.pos.y = { 0.0f };

        m_gameState = GameState::Play;
    }
}

void TRON::OnKeyPressedEvent(const elv::events::KeyPressedEvent& e)
{
    switch (e.key) {
    case elv::key::Space:
        OnPause();
        break;
    case elv::key::P:
        OnStartGame();
        break;
    case elv::key::R:
        OnRestart();
        break;
    case elv::key::Q:
        m_running = false;
        break;
    }
}
