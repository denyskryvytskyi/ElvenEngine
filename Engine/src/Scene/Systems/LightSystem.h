#pragma once

#include "Scene/ComponentSystem.h"

namespace elv {

class LightSystem : public ecs::IComponentSystem {
public:
    void OnUpdate(float dt) override;
};

} // namespace elv
