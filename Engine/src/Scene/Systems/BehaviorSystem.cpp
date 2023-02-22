#include "BehaviorSystem.h"
#include "Scene/Behavior.h"
#include "Scene/Components/SceneComponents.h"
#include "Scene/Scene.h"

namespace elv {
BehaviorSystem::BehaviorSystem(Scene* scenePtr)
    : ecs::IComponentSystem(scenePtr)
{
}

void BehaviorSystem::OnInit()
{
    m_behaviorsPool = m_pScene->GetComponentPool<BehaviorComponent>();
}

void BehaviorSystem::OnShutdown()
{
    auto& behaviorComponents = m_behaviorsPool->GetComponents();
    for (uint32_t i = 0; i < behaviorComponents.size(); ++i) {
        auto& component = behaviorComponents[i];
        if (component.behavior) {
            component.behavior->OnDestroy();
        }
    }
}

void BehaviorSystem::OnUpdate(float dt)
{
    auto& behaviorComponents = m_behaviorsPool->GetComponents();
    for (uint32_t i = 0; i < behaviorComponents.size(); ++i) {
        auto& component = behaviorComponents[i];
        if (component.behavior) {
            if (component.isEnabled) {
                component.behavior->OnUpdate(dt);
            }
        } else {
            const ecs::Entity entity = m_behaviorsPool->GetEntity(i);
            component.behavior = component.instantiateBehavior();
            component.behavior->SetEntity(entity);
            component.behavior->OnCreate();
        }
    }
}
void BehaviorSystem::OnRender(float dt)
{
    for (auto& component : m_behaviorsPool->GetComponents()) {
        if (component.behavior) {
            component.behavior->OnRender(dt);
        }
    }
}
} // namespace elv
