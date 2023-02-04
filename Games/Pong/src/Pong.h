#pragma once

#include <Core/Application.h>
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
        GameOver,
        RestartMenu
    };

public:
    void OnCreate() override;
    void OnUpdate(float dt) override;
    void OnWindowResizeApp() override;

    void OnMenuState();
    void OnPlayState();
    void OnGameOverState();
    void OnRestartMenuState();

private:
    Player m_players[2];
    elv::ecs::Entity m_ball;
    elv::ecs::Entity m_winnerEntity;
    GameState m_gameState { GameState::Menu };

    // text
    elv::ecs::Entity m_startMenuTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_gameOverTextEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::ecs::Entity m_restartMenuTextEntity { elv::ecs::INVALID_ENTITY_ID };
};
