#pragma once

#include <Elven.h>

#include "Helpers/EnvironmentSettings.h"

class MeshModelSandbox : public elv::Application {
public:
    MeshModelSandbox();

    void OnUpdate(float dt) override;
    void OnRender(float dt) override;
    void OnProcessInput(float dt) override;
#if EDITOR_MODE
    void OnImguiRender() override;
#endif

private:
    void SetEnvironment(const int envIndex);

    void SetupCubes();

private:
    elv::Timer m_timer;

    std::vector<elv::ecs::Entity> m_primitives;

    std::vector<elv::ecs::Entity> m_models;

    elv::ecs::Entity m_dirLightEntity { elv::ecs::INVALID_COMPONENT_TYPE_ID };
    elv::ecs::Entity m_flashLightEntity { elv::ecs::INVALID_COMPONENT_TYPE_ID };
    elv::ecs::Entity m_pointLightEntities[kPointLightsAmount] { elv::ecs::INVALID_COMPONENT_TYPE_ID };
};
