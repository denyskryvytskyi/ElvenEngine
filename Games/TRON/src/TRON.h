#pragma once

#include <Core/Application.h>
#include <Scene/Entity.h>

class TRON : public elv::Application {
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

    void OnMenuState();
    void OnPlayState();
    void OnGameOverState();
    void OnRestartMenuState();

private:
    elv::ecs::Entity m_players[2];
    std::string_view m_winnerName;
    GameState m_gameState { GameState::Menu };
};
