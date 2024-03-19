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
    elv::EditorCameraController m_cameraController;

    elv::Timer m_timer;

    lia::vec4 m_clearColor { 0.2f, 0.2f, 0.2f, 1.0f };

    bool m_DirLightEnabled { true };
    bool m_PointLightEnabled { false };
    bool m_SpotLightEnabled { false };

    // objects
    std::vector<elv::ecs::Entity> m_primitives;
    elv::SharedPtr<elv::Shader> m_shader { nullptr };

    // light
    elv::ecs::Entity m_dirLightEntity;
    elv::ecs::Entity m_spotLightEntity;
    elv::ecs::Entity m_pointLightEntities[kPointLightsAmount];

    elv::SharedPtr<elv::Mesh> m_lightCubeMesh { nullptr };
    elv::SharedPtr<elv::Shader> m_lightShader { nullptr };

    // Models
    std::vector<elv::ecs::Entity> m_models;
};
