#pragma once

#include <Core/Application.h>
#include <Scene/Entity.h>

class Pong : public elv::Application {
private:
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
    elv::ecs::Entity m_players[2];
    elv::ecs::Entity m_ball;
    elv::ecs::Entity m_winnerEntity;
    GameState m_gameState { GameState::Menu };
};
