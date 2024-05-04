#pragma once

#include <Elven.h>

#include "Helpers/EnvironmentSettings.h"

class PrimitivesSandbox : public elv::Application {
public:
    PrimitivesSandbox();

    void OnUpdate(float dt) override;
    void OnRender(float dt) override;
    void OnProcessInput(float dt) override;
#if MODULE_EDITOR_ENABLED
    void OnImguiRender() override;
#endif

private:
    void SetEnvironment(const int envIndex);

private:
    elv::ecs::Entity m_dirLightEntity;
    elv::ecs::Entity m_spotLightEntity;
    elv::ecs::Entity m_pointLightEntities[kPointLightsAmount];
};
