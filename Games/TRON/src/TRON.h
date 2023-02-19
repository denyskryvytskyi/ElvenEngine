#pragma once

#include <Core/Application.h>
#include <Events/KeyEvent.h>
#include <Scene/Entity.h>

class TRON : public elv::Application {
private:
    struct Player {
        elv::ecs::Entity entity;
        uint16_t score;
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

    void OnPlay();
    void OnGameOver();
    void OnPause();
    void OnStartGame();
    void OnRestart();

    void OnKeyPressedEvent(const elv::events::KeyPressedEvent& e);

private:
    Player m_players[2];
    int16_t m_winnerId { -1 }; // -1 (draw); 0(player 1); 1(player2)
    GameState m_gameState { GameState::Menu };

    // text
    elv::ecs::Entity m_startMenuText1Entity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_startMenuText2Entity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_startMenuText3Entity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_gameOverTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_restartMenuTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_pauseMenuTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_quitMenuTextEntity { elv::ecs::INVALID_ENTITY_ID };
};
