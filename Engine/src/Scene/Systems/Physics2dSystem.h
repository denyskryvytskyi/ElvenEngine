#pragma once

#include "Scene/ComponentSystem.h"
#include "Scene/Components/SceneComponents.h"

namespace elv {

class Physics2dSystem : public ecs::IComponentSystem {
public:
    void OnInit() override;
    void OnUpdate(float dt) override;
};

} // namespace elv
