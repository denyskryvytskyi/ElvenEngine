#pragma once

#include <json_fwd.hpp>

#include "Scene/Behavior.h"

namespace Elven {
struct TransformComponent {
    TransformComponent() = default;
    TransformComponent(const lia::vec3&& pos_, const lia::vec2&& scale_ = lia::vec2(), const lia::vec3&& rotation_ = lia::vec3())
        : pos(pos_)
        , scale(scale_)
        , rotation(rotation_)
    { }

    lia::vec3 pos { 0.0f, 0.0f, 0.0f };
    lia::vec2 scale { 1.0f, 1.0f };
    lia::vec3 rotation { 0.0f, 0.0f, 0.0f };
};
void to_json(nlohmann::json& j, const TransformComponent& t);
void from_json(const nlohmann::json& j, TransformComponent& t);

class Texture2D;
struct SpriteComponent {
    SpriteComponent() = default;
    SpriteComponent(std::string_view textureName_)
        : textureName(textureName_)
    { }

    std::string textureName;
    SharedPtr<Texture2D> texture { nullptr };
};

void to_json(nlohmann::json& j, const SpriteComponent& t);
void from_json(const nlohmann::json& j, SpriteComponent& t);

struct BehaviorComponent {

    ecs::IBehavior* behavior { nullptr };

    ecs::IBehavior* (*InstantiateBehavior)();
    void (*DestroyBehavior)(BehaviorComponent&);

    template<class BehaviorType>
    void Bind()
    {
        InstantiateBehavior = []() { return static_cast<ecs::IBehavior*>(new BehaviorType()); };
        DestroyBehavior = [](BehaviorComponent& component) { delete component.behavior; component.behavior = nullptr; };
    }
};

} // namespace Elven
