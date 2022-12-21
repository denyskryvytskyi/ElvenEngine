#pragma once

#include <json_fwd.hpp>

#include "Scene/Behavior.h"

namespace Elven {
struct TransformComponent {
    lia::vec3 pos { 0.0f, 0.0f, 0.0f };
    lia::vec2 scale { 1.0f, 1.0f };
    lia::vec3 rotation { 0.0f, 0.0f, 0.0f };
};
void to_json(nlohmann::json& j, const TransformComponent& t);
void from_json(const nlohmann::json& j, TransformComponent& t);

class Texture2D;
struct SpriteComponent {
    std::string textureName;
    SharedPtr<Texture2D> texture { nullptr };
};

void to_json(nlohmann::json& j, const SpriteComponent& t);
void from_json(const nlohmann::json& j, SpriteComponent& t);

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
