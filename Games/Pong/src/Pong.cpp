#include <Elven.h>

#include "Pong.h"
#include <Events/KeyEvent.h>
#include <Resources/AudioManager.h>

elv::Application* elv::CreateApplication()
{
    return new Pong();
}

namespace {
constexpr float paddleSpeed = 150.0f;
const lia::vec3 paddleScale = { 10.0f, 40.0f, 1.0f };
constexpr float paddleSizeXhalf = 5.0f;
constexpr float paddleSizeYhalf = 20.0f;
constexpr float paddleOffset = 10.0f;
const lia::vec4 paddle1Color = { 0.9f, 0.7f, 0.0f, 1.0f };
const lia::vec4 paddle2Color = { 0.3f, 0.3f, 0.8f, 1.0f };

constexpr float ballSpeed = 100.0f;
constexpr float ballSpeedIncr = 15.0f;
const lia::vec3 ballScale = { 7.0f, 7.0f, 1.0f };
const lia::vec4 ballColor = { 0.8f, 0.8f, 0.8f, 1.0f };
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
            tr.SetPositionY(std::max(tr.GetPosition().y - paddleSpeed * dt, m_paddle.bottomBoundPos));
        } else if (elv::Input::IsKeyPressed(m_paddle.upKey)) {
            tr.SetPositionY(std::min(tr.GetPosition().y + paddleSpeed * dt, m_paddle.topBoundPos));
        }
    }

private:
    PaddleComponent m_paddle;
};

struct BallComponent {
    float speed = ballSpeed;
    lia::vec3 velocity { 0.0f, 0.0f, 0.0f };
};

class BallBehavior : public elv::ecs::IBehavior {
    void OnCreate()
    {
        BallComponent& ball = GetComponent<BallComponent>();
        ball.velocity.x = elv::GetRand(0, 1) == 0 ? -1.0f : 1.0f;
        ball.velocity.y = elv::GetRand(0, 1) == 0 ? -1.0f : 1.0f;
    }

    void OnUpdate(float dt) override
    {
        elv::TransformComponent& tr = GetComponent<elv::TransformComponent>();
        BallComponent& ball = GetComponent<BallComponent>();

        tr.Translate(ball.velocity * ball.speed * dt);
    }
};

void Pong::OnCreate()
{
    elv::gAudioManager.AddSound("back", "back.mp3");
    elv::gAudioManager.SetVolume("back", 0.5f);
    elv::gAudioManager.Play("back");

    elv::gAudioManager.AddSound("hit_paddle", "hit_paddle.wav");
    elv::gAudioManager.AddSound("hit_wall", "hit_wall.wav");

    elv::Scene& scene = elv::GetScene();
    scene.RegisterComponent<PaddleComponent>();
    scene.RegisterComponent<BallComponent>();

    m_startMenuTextEntity = scene.CreateEntity();
    scene.AddComponent<elv::RectTransformComponent>(m_startMenuTextEntity, lia::vec2(37.0f, 30.0f), lia::vec2(0.6f, 0.6f));
    scene.AddComponent<elv::TextComponent>(m_startMenuTextEntity, "Press P to start the game");

    m_quitMenuTextEntity = scene.CreateEntity();
    scene.AddComponent<elv::RectTransformComponent>(m_quitMenuTextEntity, lia::vec2(37.0f, 40.0f), lia::vec2(0.6f, 0.6f));
    auto& quitText = scene.AddComponent<elv::TextComponent>(m_quitMenuTextEntity, "Press Q to exit");

    m_gameOverTextEntity = scene.CreateEntity();
    scene.AddComponent<elv::RectTransformComponent>(m_gameOverTextEntity, lia::vec2(37.0f, 30.0f), lia::vec2(0.6f, 0.6f));
    auto& gameOverText = scene.AddComponent<elv::TextComponent>(m_gameOverTextEntity);
    gameOverText.Hide();

    m_restartMenuTextEntity = scene.CreateEntity();
    scene.AddComponent<elv::RectTransformComponent>(m_restartMenuTextEntity, lia::vec2(37.0f, 40.0f), lia::vec2(0.6f, 0.6f));
    auto& restartText = scene.AddComponent<elv::TextComponent>(m_restartMenuTextEntity, "Press R to start next round");
    restartText.Hide();

    m_pauseMenuTextEntity = scene.CreateEntity();
    scene.AddComponent<elv::RectTransformComponent>(m_pauseMenuTextEntity, lia::vec2(37.0f, 30.0f), lia::vec2(0.6f, 0.6f));
    auto& pauseText = scene.AddComponent<elv::TextComponent>(m_pauseMenuTextEntity, "Press Space to resume the game");
    pauseText.Hide();

    // event handlers
    elv::events::EventHandler<elv::events::KeyPressedEvent> keyPressedHandler([this](const elv::events::KeyPressedEvent& e) { OnKeyPressedEvent(e); });
    elv::events::Subscribe<elv::events::KeyPressedEvent>(keyPressedHandler);
}

void Pong::OnUpdate(float dt)
{
    if (m_gameState == GameState::Play) {
        OnPlay();
    }
}

void Pong::OnWindowResizeApp()
{
    if (m_gameState != GameState::Menu) {

        elv::Scene& scene = elv::GetScene();
        const auto& cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();

        // fix paddles positions
        auto& player1Transform = scene.GetComponent<elv::TransformComponent>(m_players[0].entity);
        player1Transform.SetPositionX(cameraBounds.left + paddleOffset);

        auto& player2Transform = scene.GetComponent<elv::TransformComponent>(m_players[1].entity);
        player2Transform.SetPositionX(cameraBounds.right - paddleOffset);
    }
}

void Pong::OnMenu()
{
    if (m_gameState == GameState::Menu) {

        auto& scene = elv::GetScene();

        // hide start menu
        scene.DestroyEntity(m_startMenuTextEntity);
        scene.DestroyEntity(m_quitMenuTextEntity);

        const auto& cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();

        // paddle 1
        auto player1 = scene.CreateEntity();
        m_players[0].entity = player1;

        scene.AddComponent<elv::BehaviorComponent>(player1).Bind<PaddleBehavior>();

        auto& transformPlayer1 = scene.AddComponent<elv::TransformComponent>(player1);
        transformPlayer1.SetPosition({ cameraBounds.left + paddleOffset, 0.0f, 0.0f });
        transformPlayer1.SetScale(paddleScale);

        auto& quadPlayer1 = scene.AddComponent<elv::QuadComponent>(player1);
        quadPlayer1.color = paddle1Color;

        auto& paddlePlayer1 = scene.AddComponent<PaddleComponent>(player1);

        paddlePlayer1.topBoundPos = cameraBounds.top - paddleSizeYhalf;
        paddlePlayer1.bottomBoundPos = cameraBounds.bottom + paddleSizeYhalf;

        scene.AddComponent<elv::RectTransformComponent>(player1, lia::vec2(0.0f));
        scene.AddComponent<elv::TextComponent>(player1, "0", paddle1Color);

        // paddle 2
        auto player2 = scene.CreateEntity();
        m_players[1].entity = player2;

        scene.AddComponent<elv::BehaviorComponent>(player2).Bind<PaddleBehavior>();

        auto& transform_player2 = scene.AddComponent<elv::TransformComponent>(player2);
        transform_player2.SetPosition({ cameraBounds.right - paddleOffset, 0.0f, 0.0f });
        transform_player2.SetScale(paddleScale);

        auto& quad_player2 = scene.AddComponent<elv::QuadComponent>(player2);
        quad_player2.color = paddle2Color;

        auto& paddle2 = scene.AddComponent<PaddleComponent>(player2);
        paddle2.upKey = elv::key::Up;
        paddle2.downKey = elv::key::Down;
        paddle2.topBoundPos = cameraBounds.top - paddleSizeYhalf;
        paddle2.bottomBoundPos = cameraBounds.bottom + paddleSizeYhalf;

        scene.AddComponent<elv::RectTransformComponent>(player2, lia::vec2(95.0f, 0.0f));
        scene.AddComponent<elv::TextComponent>(player2, "0", paddle2Color);

        // ball
        m_ball = scene.CreateEntity();

        scene.AddComponent<elv::BehaviorComponent>(m_ball).Bind<BallBehavior>();
        scene.AddComponent<BallComponent>(m_ball);

        auto& ballTransform = scene.AddComponent<elv::TransformComponent>(m_ball);
        ballTransform.SetScale(ballScale);

        auto& ballQuad = scene.AddComponent<elv::QuadComponent>(m_ball);
        ballQuad.color = ballColor;

        m_gameState = GameState::Play;
    }
}

void Pong::OnPlay()
{
    auto& scene = elv::GetScene();

    auto& ballTransform = scene.GetComponent<elv::TransformComponent>(m_ball);

    const auto& cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();

    // Check collision with left/right borders to find winner
    int16_t winnerId = -1;
    const float ballSizeHalf = ballTransform.GetScale().x * 0.5f;
    lia::vec3 ballPos = ballTransform.GetPosition();
    if (ballPos.x + ballSizeHalf >= cameraBounds.right) {
        winnerId = 0;
    } else if (ballPos.x - ballSizeHalf <= cameraBounds.left) {
        winnerId = 1;
    }

    if (winnerId != -1) {
        ++m_players[winnerId].score;
        scene.GetComponent<elv::TextComponent>(m_players[winnerId].entity).text = fmt::format("{}", m_players[winnerId].score);
        scene.GetComponent<elv::TextComponent>(m_gameOverTextEntity).text = fmt::format("Player {} win", winnerId + 1);
        scene.GetComponent<elv::TextComponent>(m_gameOverTextEntity).Show();
        scene.GetComponent<elv::TextComponent>(m_restartMenuTextEntity).Show();
        OnGameOver();
        return;
    }

    // Check collion with top/bottom borders
    const bool isCollisionTop = ballPos.y + ballSizeHalf >= cameraBounds.top;
    const bool isCollisionBottom = ballPos.y - ballSizeHalf <= cameraBounds.bottom;
    if (isCollisionTop || isCollisionBottom) {
        // reverse Y velocity
        auto& ballComponent = scene.GetComponent<BallComponent>(m_ball);
        ballComponent.velocity.y = -ballComponent.velocity.y;
        // penetration fix
        if (isCollisionTop) {
            const float penetration = ballSizeHalf - std::abs(cameraBounds.top - ballPos.y);
            ballTransform.TranslateY(-penetration);
        } else {
            const float penetration = ballSizeHalf - std::abs(cameraBounds.bottom - ballPos.y);
            ballTransform.TranslateY(penetration);
        }

        // hit sound
        elv::gAudioManager.Play("hit_wall");
    }

    ballPos = ballTransform.GetPosition();

    // Check collision with paddle 1
    auto& paddle1 = scene.GetComponent<elv::TransformComponent>(m_players[0].entity);
    const auto& paddle1Pos = paddle1.GetPosition();

    if (ballPos.x - ballSizeHalf <= paddle1Pos.x + paddleSizeXhalf
        && ballPos.y + ballSizeHalf > paddle1Pos.y - paddleSizeYhalf
        && ballPos.y - ballSizeHalf < paddle1Pos.y + paddleSizeYhalf) {
        // reverse X velocity
        auto& ballComponent = scene.GetComponent<BallComponent>(m_ball);
        ballComponent.velocity.x = -ballComponent.velocity.x;
        // penetration fix
        const float penetration = ballSizeHalf - std::abs((ballPos.x - (paddle1Pos.x + paddleSizeXhalf)));
        ballTransform.TranslateX(penetration);

        // change ball color to paddle one
        scene.GetComponent<elv::QuadComponent>(m_ball).color = paddle1Color;
        scene.GetComponent<BallComponent>(m_ball).speed += ballSpeedIncr;

        // hit sound
        elv::gAudioManager.Play("hit_paddle");
    }

    ballPos = ballTransform.GetPosition();

    // Check collision with paddle 2
    auto& paddle2 = scene.GetComponent<elv::TransformComponent>(m_players[1].entity);
    const auto& paddle2Pos = paddle2.GetPosition();

    if (ballPos.x + ballSizeHalf >= paddle2Pos.x - paddleSizeXhalf
        && ballPos.y + ballSizeHalf > paddle2Pos.y - paddleSizeYhalf
        && ballPos.y - ballSizeHalf < paddle2Pos.y + paddleSizeYhalf) {
        // reverse X velocity
        auto& ballComponent = scene.GetComponent<BallComponent>(m_ball);
        ballComponent.velocity.x = -ballComponent.velocity.x;
        // penetration fix
        const float penetration = ballSizeHalf - std::abs(((paddle2Pos.x - paddleSizeXhalf) - ballPos.x));
        ballTransform.TranslateX(-penetration);

        // change ball color to paddle one
        scene.GetComponent<elv::QuadComponent>(m_ball).color = paddle2Color;
        scene.GetComponent<BallComponent>(m_ball).speed += ballSpeedIncr;

        // hit sound
        elv::gAudioManager.Play("hit_paddle");
    }
}

void Pong::OnGameOver()
{
    auto& scene = elv::GetScene();

    // Remove redundant components from players and reset transforms
    for (auto player : m_players) {
        scene.RemoveComponent<elv::BehaviorComponent>(player.entity);
        auto& playerTransform = scene.GetComponent<elv::TransformComponent>(player.entity);
        playerTransform.SetPositionY(0.0f);
    }
    scene.RemoveComponent<elv::BehaviorComponent>(m_ball);
    auto& ballTransform = scene.GetComponent<elv::TransformComponent>(m_ball);
    ballTransform.SetPosition({ 0.0f, 0.0f, 0.0f });
    scene.GetComponent<elv::QuadComponent>(m_ball).color = ballColor;

    m_gameState = GameState::GameOver;
}

void Pong::OnRestart()
{
    if (m_gameState == GameState::GameOver) {
        auto& scene = elv::GetScene();

        // hide text
        scene.GetComponent<elv::TextComponent>(m_gameOverTextEntity).Hide();
        scene.GetComponent<elv::TextComponent>(m_restartMenuTextEntity).Hide();

        //
        for (auto player : m_players) {
            scene.AddComponent<elv::BehaviorComponent>(player.entity).Bind<PaddleBehavior>();
        }
        scene.AddComponent<elv::BehaviorComponent>(m_ball).Bind<BallBehavior>();
        scene.GetComponent<BallComponent>(m_ball).speed = ballSpeed;

        m_gameState = GameState::Play;
    }
}

void Pong::OnPause()
{
    auto& scene = elv::GetScene();

    if (m_gameState == GameState::Play) {
        for (auto player : m_players) {
            scene.GetComponent<elv::BehaviorComponent>(player.entity).Disable();
        }
        scene.GetComponent<elv::BehaviorComponent>(m_ball).Disable();
        scene.GetComponent<elv::TextComponent>(m_pauseMenuTextEntity).Show();
        m_gameState = GameState::Pause;
    } else if (m_gameState == GameState::Pause) {
        for (auto player : m_players) {
            scene.GetComponent<elv::BehaviorComponent>(player.entity).Enable();
        }
        scene.GetComponent<elv::BehaviorComponent>(m_ball).Enable();
        scene.GetComponent<elv::TextComponent>(m_pauseMenuTextEntity).Hide();
        m_gameState = GameState::Play;
    }
}

void Pong::OnKeyPressedEvent(const elv::events::KeyPressedEvent& e)
{
    switch (e.key) {
    case elv::key::Space:
        OnPause();
        break;
    case elv::key::P:
        OnMenu();
        break;
    case elv::key::R:
        OnRestart();
        break;
    case elv::key::Q:
        m_running = false;
        break;
    }
}
