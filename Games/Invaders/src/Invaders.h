#pragma once

#include <Core/Application.h>
#include <Events/KeyEvent.h>
#include <Scene/Entity.h>

// ----------------------- Custom events -----------------------
struct PlayerHitEvent : public elv::events::Event {
public:
    PlayerHitEvent(bool isReceived, int damage)
        : isReceived(isReceived)
        , damage(damage)
    {
    }

    EVENT_TYPE("PlayerHitEvent");

public:
    bool isReceived { false }; // false - damaged enemy; true - received
    int damage { 0 };
};

struct GameOverEvent : public elv::events::Event {
public:
    EVENT_TYPE("GameOverEvent");
};

// ----------------------- Game -----------------------

class Invaders : public elv::Application {
private:
    enum class GameState {
        Menu,
        Play,
        GameOver
    };

public:
    void OnCreate() override;
    void OnUpdate(float dt) override;
    void OnWindowResizeApp() override;

private:
    void GeneratePlayerBullet() const;
    void GenerateEnemiesGrid();
    void SetupBackground(elv::Scene& scene);

    void OnKeyPressedEvent(const elv::events::KeyPressedEvent& e);
    void OnRoundStart();
    void OnGameOver(const GameOverEvent& e);
    void OnPlayerHitEvent(const PlayerHitEvent& e);

private:
    elv::ecs::Entity m_playerEntity { elv::ecs::INVALID_ENTITY_ID };
    GameState m_gameState { GameState::Menu };
    int m_destroyedWaveEnemies { 0 };
    std::vector<elv::ecs::Entity> m_backgroundTiles;

    elv::Timer m_nextWaveTimer;
    bool isNextWaveWait { false };

    // labels
    elv::ecs::Entity m_gameName { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_startMenuTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_quitMenuTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_restartMenuTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_gameOverTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_nextWaveTextEntity { elv::ecs::INVALID_ENTITY_ID };
};
