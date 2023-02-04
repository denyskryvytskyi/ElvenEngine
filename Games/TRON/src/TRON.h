#pragma once

#include <Core/Application.h>
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
        GameOver,
        RestartMenu
    };

public:
    void OnCreate() override;
    void OnUpdate(float dt) override;

    void OnMenuState();
    void OnPlayState();
    void OnGameOverState();
    void OnRestartMenuState();

private:
    Player m_players[2];
    int16_t m_winnerId { -1 }; // -1 (draw); 0(player 1); 1(player2)
    GameState m_gameState { GameState::Menu };

    // text
    elv::ecs::Entity m_startMenuText1Entity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_startMenuText2Entity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_gameOverTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_restartMenuTextEntity { elv::ecs::INVALID_ENTITY_ID };
};
