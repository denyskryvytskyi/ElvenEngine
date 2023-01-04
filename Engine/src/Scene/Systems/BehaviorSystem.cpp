#include "BehaviorSystem.h"
#include "Scene/Behavior.h"
#include "Scene/Components/SceneComponents.h"
#include "Scene/Scene.h"

namespace Elven {
BehaviorSystem::BehaviorSystem(Scene* scenePtr)
    : ecs::IComponentSystem(scenePtr)
{
}

void BehaviorSystem::OnShutdown()
{
    auto& behaviorComponents = m_pScene->GetComponents<BehaviorComponent>();
    for (uint32_t i = 0; i < behaviorComponents.size(); ++i) {
        auto& component = behaviorComponents[i];
        if (component.behavior) {
            component.behavior->OnDestroy();
            component.DestroyBehavior(component);
        }
    }
}

void BehaviorSystem::OnUpdate(float dt)
{
    auto& behaviorComponents = m_pScene->GetComponents<BehaviorComponent>();
    for (uint32_t i = 0; i < behaviorComponents.size(); ++i) {
        auto& component = behaviorComponents[i];
        if (component.behavior) {
            component.behavior->OnUpdate(dt);
        } else {
            const ecs::Entity entity = m_pScene->GetEntity<BehaviorComponent>(i);
            component.behavior = component.InstantiateBehavior();
            component.behavior->SetEntity(entity);
            component.behavior->OnCreate();
        }
    }
}
void BehaviorSystem::OnRender(float dt)
{
    for (auto& component : m_pScene->GetComponents<BehaviorComponent>()) {
        if (component.behavior) {
            component.behavior->OnRender(dt);
        }
    }
}
} // namespace Elven
