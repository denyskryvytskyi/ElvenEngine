#pragma once

#include <Core/Application.h>
#include <Scene/Entity.h>

class TRON : public elv::Application {
private:
    enum class GameState {
        Menu,
        Play,
        GameOver
    };

public:
    void OnCreate() override;
    void OnUpdate(float dt) override;

    void OnMenuState();
    void OnPlayState();
    void OnGameOverState();

private:
    elv::ecs::Entity m_players[2];
    elv::ecs::Entity m_winnerEntity;
    GameState m_gameState { GameState::Menu };
};
