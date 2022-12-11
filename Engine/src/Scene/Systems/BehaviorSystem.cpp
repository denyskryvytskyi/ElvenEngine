#include "BehaviorSystem.h"
#include "Scene/Behavior.h"
#include "Scene/Components/SceneComponents.h"
#include "Scene/Scene.h"

namespace Elven {
BehaviorSystem::BehaviorSystem(Scene* scenePtr)
    : ecs::IComponentSystem(scenePtr)
{
}

void BehaviorSystem::OnUpdate(float dt)
{
    for (auto& component : m_pScene->GetComponents<BehaviorComponent>()) {
        component.m_behavior->OnUpdate(dt);
    }
}
void BehaviorSystem::OnRender(float dt)
{
    for (auto& component : m_pScene->GetComponents<BehaviorComponent>()) {
        component.m_behavior->OnRender(dt);
    }
}
} // namespace Elven
