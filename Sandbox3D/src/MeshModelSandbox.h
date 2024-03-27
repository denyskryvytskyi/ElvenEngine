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
};
