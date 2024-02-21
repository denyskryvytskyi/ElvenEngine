#pragma once

#include <Core/Application.h>
#include <Events/KeyEvent.h>
#include <Scene/Entity.h>

class Pong : public elv::Application {
private:
    struct Player {
        elv::ecs::Entity entity { elv::ecs::INVALID_ENTITY_ID };
        uint16_t score { 0 };
    };
    enum class GameState {
        Menu,
        Play,
        Pause,
        GameOver
    };

public:
    void OnCreate() override;
    void OnUpdate(float dt) override;
    void OnWindowResizeApp() override;

    void OnMenu();
    void OnPlay();
    void OnGameOver();
    void OnRestart();
    void OnPause();

    void OnKeyPressedEvent(const elv::events::KeyPressedEvent& e);

private:
    Player m_players[2];
    elv::ecs::Entity m_ball { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_winnerEntity { elv::ecs::INVALID_ENTITY_ID };
    GameState m_gameState { GameState::Menu };

    // text
    elv::ecs::Entity m_startMenuTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_quitMenuTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_pauseMenuTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_restartMenuTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_gameOverTextEntity { elv::ecs::INVALID_ENTITY_ID };
};
