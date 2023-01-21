#pragma once

#include <json_fwd.hpp>

#include "Scene/Behavior.h"

namespace elv {
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

struct QuadComponent {
    QuadComponent() = default;
    QuadComponent(const lia::vec4 color_)
        : color(color_)
    { }

    lia::vec4 color;
};
void to_json(nlohmann::json& j, const QuadComponent& t);
void from_json(const nlohmann::json& j, QuadComponent& t);

class Texture2D;
struct SpriteComponent {
    SpriteComponent() = default;
    SpriteComponent(std::string_view texture_name, std::string_view texture_path)
        : textureName(texture_name)
        , texturePath(texture_path)
    {
        LoadTexture();
    }

    // Also calls LoadTexture
    void SetTexture(std::string_view texture_name, std::string_view texture_path)
    {
        textureName = texture_name;
        texturePath = texture_path;
        LoadTexture();
    }

    // Load texture if it has textureName and texturePath
    // Actually is used only in case of scene deserialization
    void LoadTexture();

public:
    std::string textureName;
    std::string texturePath;
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

} // namespace elv
