#include <Elven.h>

#include "TRON.h"

elv::Application* elv::CreateApplication()
{
    return new TRON();
}

namespace {
constexpr float segmentSize = 1.0f;
std::vector<elv::ecs::Entity> m_lineSegments;
} // namespace

struct LightCycleComponent {
    lia::vec4 lineColor { .0f, 0.2f, 0.8f, 1.0f };
    lia::vec3 startVelocity { 1.0f, 0.0f, 0.0f };
    elv::Keycode upKey { elv::key::W };
    elv::Keycode downKey { elv::key::S };
    elv::Keycode leftKey { elv::key::A };
    elv::Keycode rightKey { elv::key::D };
};

// AABB - AABB collision
static bool CheckCollision(const elv::TransformComponent& lightcycle, const elv::TransformComponent& lineSegment)
{
    // both x and y axis
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
        elv::TransformComponent& tr = GetComponent<elv::TransformComponent>();

        if (elv::Input::IsKeyPressed(m_lightcycle.leftKey) && m_velocity.x != 1.0f) {
            m_velocity.y = 0.0f;
            m_velocity.x = -1.0f;
        } else if (elv::Input::IsKeyPressed(m_lightcycle.rightKey) && m_velocity.x != -1.0f) {
            m_velocity.y = 0.0f;
            m_velocity.x = 1.0f;
        } else if (elv::Input::IsKeyPressed(m_lightcycle.downKey) && m_velocity.y != 1.0f) {
            m_velocity.y = -1.0f;
            m_velocity.x = 0.0f;
        } else if (elv::Input::IsKeyPressed(m_lightcycle.upKey) && m_velocity.y != -1.0f) {
            m_velocity.y = 1.0f;
            m_velocity.x = 0.0f;
        }

        if (timer.ElapsedMs() >= m_speedMs) {
            SpawnLineSegment(tr.pos);
            tr.pos += m_velocity * segmentSize;
            timer.Restart();
        }
    }

private:
    void SpawnLineSegment(const lia::vec3& lightcyclePos)
    {
        const auto entity = p_Scene->CreateEntity();

        auto& transform_segment = p_Scene->AddComponent<elv::TransformComponent>(entity);
        transform_segment.pos = lightcyclePos;
        transform_segment.scale = { segmentSize, segmentSize };

        auto& quad_segment = p_Scene->AddComponent<elv::QuadComponent>(entity);
        quad_segment.color = m_lightcycle.lineColor;

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
    transformComponent.scale = { 700.0f, 700.0f };
    scene.AddComponent<elv::SpriteComponent>(back_entity, "background", "background.png");
}

void TRON::OnUpdate(float dt)
{
    elv::Scene& scene = elv::GetScene();

    if (m_gameState == GameState::Menu) {
        OnMenuState();
    } else if (m_gameState == GameState::Play) {
        OnPlayState();
    } else if (m_gameState == GameState::GameOver) {
        OnGameOverState();
    }
}

void TRON::OnMenuState()
{
    auto& scene = elv::GetScene();

    if (elv::Input::IsKeyPressed(elv::key::P)) {
        //  1
        auto player1 = scene.CreateEntity();
        m_players[0] = player1;
        scene.AddComponent<elv::BehaviorComponent>(player1).Bind<LightCycleBehavior>();
        auto& transform_player1 = scene.AddComponent<elv::TransformComponent>(player1);
        transform_player1.pos = { -50.0f, 0.0f, 0.0f };
        transform_player1.scale = { segmentSize, segmentSize };
        auto& quad_player1 = scene.AddComponent<elv::QuadComponent>(player1);
        quad_player1.color = { 0.8f, 0.8f, 0.8f, 1.0f };
        scene.AddComponent<LightCycleComponent>(player1);

        // 2
        auto player2 = scene.CreateEntity();
        m_players[1] = player2;
        scene.AddComponent<elv::BehaviorComponent>(player2).Bind<LightCycleBehavior>();
        auto& transform_player2 = scene.AddComponent<elv::TransformComponent>(player2);
        transform_player2.pos = { 50.0f, 0.0f, 0.0f };
        transform_player2.scale = { segmentSize, segmentSize };
        auto& quad_player2 = scene.AddComponent<elv::QuadComponent>(player2);
        quad_player2.color = { 0.8f, 0.8f, 0.8f, 1.0f };
        auto& lightcycleComponent = scene.AddComponent<LightCycleComponent>(player2);
        lightcycleComponent.lineColor = { 0.7f, 0.5f, 0.0f, 1.0f };
        lightcycleComponent.startVelocity = { -1.0f, 0.0f, 0.0f };
        lightcycleComponent.upKey = elv::key::Up;
        lightcycleComponent.downKey = elv::key::Down;
        lightcycleComponent.leftKey = elv::key::Left;
        lightcycleComponent.rightKey = elv::key::Right;
        m_gameState = GameState::Play;
    }
}

void TRON::OnPlayState()
{
    auto& scene = elv::GetScene();

    // Check collisions between players and walls
    for (size_t i = 0; i < 2; i++) {
        auto playerEntity = m_players[i];
        auto& playerTr = scene.GetComponent<elv::TransformComponent>(playerEntity);

        // Check collion with boundaries
        auto cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();
        if (playerTr.pos.x <= cameraBounds.left || playerTr.pos.x >= cameraBounds.right
            || playerTr.pos.y >= cameraBounds.top || playerTr.pos.y <= cameraBounds.bottom) {
            m_gameState = GameState::GameOver;
            m_winnerEntity = i == 0 ? m_players[1] : m_players[0];
            break;
        }

        // Check collision with lines
        for (auto segmentEntity : m_lineSegments) {
            auto& segmentTr = scene.GetComponent<elv::TransformComponent>(segmentEntity);
            if (CheckCollision(playerTr, segmentTr)) {
                m_gameState = GameState::GameOver;
                m_winnerEntity = i == 0 ? m_players[1] : m_players[0];
                break;
            }
        }
    }
}

void TRON::OnGameOverState()
{
    auto& scene = elv::GetScene();

    // Remove redundant components from players
    for (auto player : m_players) {
        scene.RemoveComponent<elv::BehaviorComponent>(player);
    }
    EL_INFO("GAME OVER");
    EL_INFO("Player {0} win!", m_winnerEntity);
    EL_INFO("Press R to restart or E to exit.");

    if (elv::Input::IsKeyPressed(elv::key::R)) {
        // destroy lines
        for (auto segmentEntity : m_lineSegments) {
            scene.DestroyEntity(segmentEntity);
        }
        m_lineSegments.clear();
        // destroy players
        for (auto player : m_players) {
            scene.DestroyEntity(player);
        }

        m_gameState = GameState::Menu;
    }
}
