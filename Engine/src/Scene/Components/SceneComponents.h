#pragma once

#include "Scene/Behavior.h"

namespace Elven {
struct TransformComponent {
    lia::vec3 pos { 0.0f, 0.0f, 0.0f };
    lia::vec2 scale { 1.0f, 1.0f };
    lia::vec3 rotation { 0.0f, 0.0f, 0.0f };
};

class Texture2D;
struct SpriteComponent {
    SharedPtr<Texture2D> m_texture { nullptr };
};

// Custom behavior component, that is unique per component, not per component type as in ComponentSystem
struct BehaviorComponent {
    template<typename BehaviorType>
    void AddBehavior()
    {
        m_behavior = MakeUniquePtr<BehaviorType>();
    }
    UniquePtr<ecs::IBehavior> m_behavior { nullptr };
};

} // namespace Elven
