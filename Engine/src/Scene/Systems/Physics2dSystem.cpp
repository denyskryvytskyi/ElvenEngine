#include "Physics2dSystem.h"

#include "Events/EventManager.h"
#include "Events/PhysicsEvent.h"

#include "Scene/Components/TransformComponent.h"

namespace elv {
void Physics2dSystem::OnInit()
{
}

void Physics2dSystem::OnUpdate(float dt)
{
    auto aabbPool = m_pScene->GetComponentPool<AABBComponent>();
    auto transformPool = m_pScene->GetComponentPool<TransformComponent>();

    for (std::uint32_t i = 0; i < aabbPool->Size(); ++i) {
        for (std::uint32_t j = i + 1; j < aabbPool->Size(); ++j) {
            const auto& aabb1Entity = aabbPool->GetEntity(i);
            const auto& aabb2Entity = aabbPool->GetEntity(j);
            if (!m_pScene->IsEntityAlive(aabb1Entity) || !m_pScene->IsEntityAlive(aabb2Entity)) {
                continue;
            }

            const auto& aabb1 = aabbPool->GetByIndex(i);
            const auto& aabb2 = aabbPool->GetByIndex(j);

            const auto& transform1 = transformPool->GetComponent(aabb1Entity);
            const auto& transform2 = transformPool->GetComponent(aabb2Entity);

            // Check collision

            const lia::vec2 halfAabbSize1 = aabb1.size * 0.5f;
            const lia::vec2 halfAabbSize2 = aabb2.size * 0.5f;

            /*
                check whether right side of the 1st aabb is greater then left side of the 2nd aabb
                and check whether right side of the 2nd aabb is greater then left side of the 1st aabb
            */
            const bool isCollisionX = transform1.GetPosition().x + halfAabbSize1.x >= transform2.GetPosition().x - halfAabbSize2.x
                && transform1.GetPosition().x - halfAabbSize1.x <= transform2.GetPosition().x + halfAabbSize2.x;
            /*
                check whether top side of the 1st aabb is greater then bottom side of the 2nd aabb
                and check whether top side of the 2nd aabb is greater then bottom side of the 1st aabb
            */
            const bool isCollisionY = transform1.GetPosition().y + halfAabbSize1.y >= transform2.GetPosition().y - halfAabbSize2.y
                && transform1.GetPosition().y - halfAabbSize1.y <= transform2.GetPosition().y + halfAabbSize2.y;

            if (isCollisionX && isCollisionY) {
                events::TriggerEvent(events::CollisionEnter2dEvent(aabb1Entity, aabb2Entity));
            }
        }
    }
}
} // namespace elv
