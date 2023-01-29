#include <Elven.h>

#include "TRON.h"

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
    float m_speedMs { 10.0f }; // ms to generate next segment
    elv::Timer timer;

    LightCycleComponent m_lightcycle;
};

void TRON::OnCreate()
{
    elv::Scene& scene = elv::GetScene();
    scene.RegisterComponent<LightCycleComponent>();

    // background
    auto back_entity = scene.CreateEntity();
    auto& transformComponent = scene.AddComponent<elv::TransformComponent>(back_entity);
    transformComponent.scale = { 700.0f, 700.0f, 1.0f };
    scene.AddComponent<elv::SpriteComponent>(back_entity, "background", "background.png");
}

void TRON::OnUpdate(float dt)
{
    if (m_gameState == GameState::Menu) {
        OnMenuState();
    } else if (m_gameState == GameState::Play) {
        OnPlayState();
    } else if (m_gameState == GameState::GameOver) {
        OnGameOverState();
    } else if (m_gameState == GameState::RestartMenu) {
        OnRestartMenuState();
    }
}

void TRON::OnMenuState()
{
    if (elv::Input::IsKeyPressed(elv::key::P)) {
        auto& scene = elv::GetScene();
        auto cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();

        // player 1
        auto player1 = scene.CreateEntity();
        m_players[0] = player1;

        scene.AddComponent<elv::BehaviorComponent>(player1).Bind<LightCycleBehavior>();

        auto& transformPlayer1 = scene.AddComponent<elv::TransformComponent>(player1);
        transformPlayer1.pos = { cameraBounds.left + 20.0f, 0.0f, 0.0f };
        transformPlayer1.scale = { segmentSize, segmentSize, 1.0f };

        auto& quadPlayer1 = scene.AddComponent<elv::QuadComponent>(player1);
        quadPlayer1.color = lightCycleColor;
        scene.AddComponent<LightCycleComponent>(player1);

        // player 2
        auto player2 = scene.CreateEntity();
        m_players[1] = player2;

        scene.AddComponent<elv::BehaviorComponent>(player2).Bind<LightCycleBehavior>();
        auto& transformPlayer2 = scene.AddComponent<elv::TransformComponent>(player2);

        transformPlayer2.pos = { cameraBounds.right - 20.0f, 0.0f, 0.0f };
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

        //
        m_gameState = GameState::Play;
    }
}

void TRON::OnPlayState()
{
    auto& scene = elv::GetScene();

    bool isGameOver = false;
    // Check collisions
    for (size_t i = 0; i < 2; i++) {
        auto playerEntity = m_players[i];
        auto& playerTransform = scene.GetComponent<elv::TransformComponent>(playerEntity);

        // Check collion with boundaries
        auto cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();
        if (playerTransform.pos.x <= cameraBounds.left || playerTransform.pos.x >= cameraBounds.right
            || playerTransform.pos.y >= cameraBounds.top || playerTransform.pos.y <= cameraBounds.bottom) {
            if (isGameOver) { // there isn't a winner
                m_winnerName = "";
                break;
            } else {
                isGameOver = true;
                m_winnerName = i == 0 ? "Player 2" : "PLayer 1";
            }
        }

        // Check collision with lines
        for (auto segmentEntity : m_lineSegments) {
            auto& segmentTransform = scene.GetComponent<elv::TransformComponent>(segmentEntity);
            if (CheckCollision(playerTransform, segmentTransform)) {
                if (isGameOver) { // there isn't a winner
                    m_winnerName = "";
                    break;
                } else {
                    isGameOver = true;
                    m_winnerName = i == 0 ? "Player 2" : "PLayer 1";
                }
            }
        }
    }

    if (isGameOver) {
        m_gameState = GameState::GameOver;
    }
}

void TRON::OnGameOverState()
{
    auto& scene = elv::GetScene();

    // Remove redundant components from players
    for (auto player : m_players) {
        scene.RemoveComponent<elv::BehaviorComponent>(player);
    }

    if (m_winnerName.empty()) {
        EL_INFO("DRAW");
    } else {
        EL_INFO("Player {0} win!", m_winnerName);
    }
    EL_INFO("Press R to restart or E to exit.");

    m_gameState = GameState::RestartMenu;
}

void TRON::OnRestartMenuState()
{
    if (elv::Input::IsKeyPressed(elv::key::R)) {
        auto& scene = elv::GetScene();

        // destroy lines
        for (auto segmentEntity : m_lineSegments) {
            scene.DestroyEntity(segmentEntity);
        }
        m_lineSegments.clear();

        // Reset players
        for (auto player : m_players) {
            scene.AddComponent<elv::BehaviorComponent>(player).Bind<LightCycleBehavior>();
        }

        auto cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();

        auto& player1Transform = scene.GetComponent<elv::TransformComponent>(m_players[0]);
        player1Transform.pos.x = cameraBounds.left + paddleStartPosOffset;

        auto& player2Transform = scene.GetComponent<elv::TransformComponent>(m_players[1]);
        player2Transform.pos.x = cameraBounds.right - paddleStartPosOffset;

        m_gameState = GameState::Play;
    }
}
