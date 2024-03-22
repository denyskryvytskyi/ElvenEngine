#pragma once

#include "Scene/ComponentSystem.h"

namespace elv {

class Shader;
class Mesh;

class RenderSystem : public ecs::IComponentSystem {
public:
    void OnInit() override;
    void OnRender(float dt) override;

private:
    SharedPtr<Shader> m_shader { nullptr };
    SharedPtr<Shader> m_lightShader { nullptr };

    // debug mesh for light
    SharedPtr<Mesh> m_debugLightMesh { nullptr };
};

} // namespace elv
