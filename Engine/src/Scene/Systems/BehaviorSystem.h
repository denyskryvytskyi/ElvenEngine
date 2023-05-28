#pragma once

#include "Scene/ComponentSystem.h"
#include "Scene/Components/SceneComponents.h"

namespace elv {
class Scene;
/* BehaviorSystem responsible for dispatching engine/game events
(like update, render, input, physics collision detection etc.) to the BehaviorComponent's.
*/
class BehaviorSystem final : public ecs::IComponentSystem {
public:
    void OnInit() override;
    void OnShutdown() override;
    void OnUpdate(float dt) override;
    void OnRender(float dt) override;

private:
    SharedPtr<ecs::ComponentPool<BehaviorComponent>> m_behaviorsPool { nullptr };
};
} // namespace elv
