#include <Elven.h>

#include "Pong.h"

elv::Application* elv::CreateApplication()
{
    return new Pong();
}

namespace {
constexpr float paddleSpeed = 100.0f;
const lia::vec3 paddleScale = { 10.0f, 40.0f, 1.0f };
constexpr float paddleSizeXhalf = 5.0f;
constexpr float paddleSizeYhalf = 20.0f;
constexpr float paddleOffset = 10.0f;

constexpr float ballSpeed = 100.0f;
const lia::vec3 ballScale = { 7.0f, 7.0f, 1.0f };

const lia::vec4 objColor = { 0.8f, 0.8f, 0.8f, 1.0f };

} // namespace

struct PaddleComponent {
    elv::Keycode upKey { elv::key::W };
    elv::Keycode downKey { elv::key::S };

    float topBoundPos { 0.0f };
    float bottomBoundPos { 0.0f };
};

class PaddleBehavior : public elv::ecs::IBehavior {
    void OnCreate() override
    {
        m_paddle = GetComponent<PaddleComponent>();
    }

    void OnUpdate(float dt) override
    {
        elv::TransformComponent& tr = GetComponent<elv::TransformComponent>();

        if (elv::Input::IsKeyPressed(m_paddle.downKey)) {
            tr.pos.y = std::max(tr.pos.y - paddleSpeed * dt, m_paddle.bottomBoundPos);
        } else if (elv::Input::IsKeyPressed(m_paddle.upKey)) {
            tr.pos.y = std::min(tr.pos.y + paddleSpeed * dt, m_paddle.topBoundPos);
        }
    }

private:
    PaddleComponent m_paddle;
};

struct BallComponent {
    lia::vec3 velocity { 0.0f, 0.0f, 0.0f };
};

class BallBehavior : public elv::ecs::IBehavior {
    void OnCreate()
    {
        BallComponent& ball = GetComponent<BallComponent>();
        ball.velocity.x = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) > 0.5f ? -1.0f : 1.0f;
        ball.velocity.y = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)) > 0.5f ? -1.0f : 1.0f;
    }

    void OnUpdate(float dt) override
    {
        elv::TransformComponent& tr = GetComponent<elv::TransformComponent>();
        BallComponent& ball = GetComponent<BallComponent>();

        tr.pos += ball.velocity * ballSpeed * dt;
    }
};

void Pong::OnCreate()
{
    elv::Scene& scene = elv::GetScene();
    scene.RegisterComponent<PaddleComponent>();
    scene.RegisterComponent<BallComponent>();
}

void Pong::OnUpdate(float dt)
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

void Pong::OnMenuState()
{
    if (elv::Input::IsKeyPressed(elv::key::P)) {
        auto& scene = elv::GetScene();

        auto cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();

        // paddle 1
        auto player1 = scene.CreateEntity();
        m_players[0] = player1;

        scene.AddComponent<elv::BehaviorComponent>(player1).Bind<PaddleBehavior>();

        auto& transformPlayer1 = scene.AddComponent<elv::TransformComponent>(player1);
        transformPlayer1.pos = { cameraBounds.left + paddleOffset, 0.0f, 0.0f };
        transformPlayer1.scale = paddleScale;

        auto& quad_player1 = scene.AddComponent<elv::QuadComponent>(player1);
        quad_player1.color = objColor;

        auto& paddle1 = scene.AddComponent<PaddleComponent>(player1);

        paddle1.topBoundPos = cameraBounds.top - paddleSizeYhalf;
        paddle1.bottomBoundPos = cameraBounds.bottom + paddleSizeYhalf;

        // paddle 2
        auto player2 = scene.CreateEntity();
        m_players[1] = player2;

        scene.AddComponent<elv::BehaviorComponent>(player2).Bind<PaddleBehavior>();

        auto& transform_player2 = scene.AddComponent<elv::TransformComponent>(player2);
        transform_player2.pos = { cameraBounds.right - paddleOffset, 0.0f, 0.0f };
        transform_player2.scale = paddleScale;

        auto& quad_player2 = scene.AddComponent<elv::QuadComponent>(player2);
        quad_player2.color = objColor;

        auto& paddle2 = scene.AddComponent<PaddleComponent>(player2);
        paddle2.upKey = elv::key::Up;
        paddle2.downKey = elv::key::Down;
        paddle2.topBoundPos = cameraBounds.top - paddleSizeYhalf;
        paddle2.bottomBoundPos = cameraBounds.bottom + paddleSizeYhalf;

        // ball
        m_ball = scene.CreateEntity();

        scene.AddComponent<elv::BehaviorComponent>(m_ball).Bind<BallBehavior>();
        scene.AddComponent<BallComponent>(m_ball);

        auto& ballTransform = scene.AddComponent<elv::TransformComponent>(m_ball);
        ballTransform.scale = ballScale;

        auto& ballQuad = scene.AddComponent<elv::QuadComponent>(m_ball);
        ballQuad.color = objColor;

        m_gameState = GameState::Play;
    }
}

void Pong::OnPlayState()
{
    auto& scene = elv::GetScene();

    auto& ballTransform = scene.GetComponent<elv::TransformComponent>(m_ball);

    auto cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();

    // Check collision with left/right borders
    const float ballSizeHalf = ballTransform.scale.x * 0.5f;
    if (ballTransform.pos.x + ballSizeHalf >= cameraBounds.right) {
        EL_INFO("PLAYER 1 WIN!");
        m_gameState = GameState::GameOver;
    } else if (ballTransform.pos.x - ballSizeHalf <= cameraBounds.left) {
        EL_INFO("PLAYER 2 WIN!");
        m_gameState = GameState::GameOver;
    }

    // Check collion with top/bottom borders
    else if (ballTransform.pos.y + ballSizeHalf >= cameraBounds.top || ballTransform.pos.y - ballSizeHalf <= cameraBounds.bottom) {
        // reverse Y velocity
        auto& ballComponent = scene.GetComponent<BallComponent>(m_ball);
        ballComponent.velocity.y = -ballComponent.velocity.y;
    }

    // Check collision with paddle 1
    auto& paddle1 = scene.GetComponent<elv::TransformComponent>(m_players[0]);

    if (ballTransform.pos.x - ballSizeHalf <= paddle1.pos.x + paddleSizeXhalf
        && ballTransform.pos.y + ballSizeHalf > paddle1.pos.y - paddleSizeYhalf
        && ballTransform.pos.y - ballSizeHalf < paddle1.pos.y + paddleSizeYhalf) {
        // reverse X velocity
        auto& ballComponent = scene.GetComponent<BallComponent>(m_ball);
        ballComponent.velocity.x = -ballComponent.velocity.x;
        // penetration fix
        const float penetration = std::abs(ballSizeHalf - (ballTransform.pos.x - (paddle1.pos.x + paddleSizeXhalf)));
        ballTransform.pos.x += penetration;
    }

    // Check collision with paddle 2
    auto& paddle2 = scene.GetComponent<elv::TransformComponent>(m_players[1]);
    if (ballTransform.pos.x + ballSizeHalf >= paddle2.pos.x - paddleSizeXhalf
        && ballTransform.pos.y + ballSizeHalf > paddle2.pos.y - paddleSizeYhalf
        && ballTransform.pos.y - ballSizeHalf < paddle2.pos.y + paddleSizeYhalf) {
        // reverse X velocity
        auto& ballComponent = scene.GetComponent<BallComponent>(m_ball);
        ballComponent.velocity.x = -ballComponent.velocity.x;
        // penetration fix
        const float penetration = std::abs(ballSizeHalf - ((paddle2.pos.x - paddleSizeXhalf) - ballTransform.pos.x));
        ballTransform.pos.x -= penetration;
    }
}

void Pong::OnGameOverState()
{
    auto& scene = elv::GetScene();

    // Remove redundant components from players and reset transforms
    for (auto player : m_players) {
        scene.RemoveComponent<elv::BehaviorComponent>(player);
        auto& playerTransform = scene.GetComponent<elv::TransformComponent>(player);
        playerTransform.pos.y = 0.0f;
    }
    scene.RemoveComponent<elv::BehaviorComponent>(m_ball);
    auto& ballTransform = scene.GetComponent<elv::TransformComponent>(m_ball);
    ballTransform.pos = { 0.0f, 0.0f, 0.0f };

    EL_INFO("GAME OVER");
    EL_INFO("Player {0} win!", m_winnerEntity);
    EL_INFO("Press R to restart or E to exit.");

    m_gameState = GameState::RestartMenu;
}

void Pong::OnRestartMenuState()
{
    if (elv::Input::IsKeyPressed(elv::key::R)) {
        auto& scene = elv::GetScene();

        for (auto player : m_players) {
            scene.AddComponent<elv::BehaviorComponent>(player).Bind<PaddleBehavior>();
        }
        scene.AddComponent<elv::BehaviorComponent>(m_ball).Bind<BallBehavior>();

        m_gameState = GameState::Play;
    }
}
