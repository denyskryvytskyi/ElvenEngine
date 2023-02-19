#pragma once

#include <json_fwd.hpp>

#include "Scene/Behavior.h"

#include "Renderer/Camera.h"

namespace elv {
struct TransformComponent {
    TransformComponent() = default;
    TransformComponent(const lia::vec3& pos_, const lia::vec3& scale_ = { 1.0f }, const lia::vec3& rotation_ = { 0.0f })
        : pos(pos_)
        , scale(scale_)
        , rotation(rotation_)
    { }

public:
    lia::vec3 pos;
    lia::vec3 scale { 1.0f };
    lia::vec3 rotation;
};
void to_json(nlohmann::json& j, const TransformComponent& t);
void from_json(const nlohmann::json& j, TransformComponent& t);

struct QuadComponent {
    QuadComponent() = default;
    QuadComponent(const lia::vec4 color_)
        : color(color_)
    { }

    lia::vec4 color { 1.0f };
};
void to_json(nlohmann::json& j, const QuadComponent& t);
void from_json(const nlohmann::json& j, QuadComponent& t);

class Texture2D;
struct SpriteComponent {
    SpriteComponent() = default;

    /**
     * Set and load texture for the sprite
     *
     * @param texture_name Texture name to store in manager
     * @param texture_path Relative path to the texture file
     */
    SpriteComponent(std::string_view texture_name, std::string_view texture_path)
        : textureName(texture_name)
        , texturePath(texture_path)
    {
        LoadTexture();
    }

    /**
     * Set and load texture for the sprite
     *
     * @param texture_name Texture name to store in manager
     * @param texture_path Relative path to the texture file
     */
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
    template<class BehaviorType>
    void Bind()
    {
        instantiateBehavior = []() { return MakeUniquePtr<BehaviorType>(); };
    }

    void Enable()
    {
        isEnabled = true;
    }

    void Disable()
    {
        isEnabled = false;
    }

public:
    std::unique_ptr<ecs::IBehavior> behavior;
    std::function<std::unique_ptr<ecs::IBehavior>()> instantiateBehavior;
    bool isEnabled { true };
};

struct CameraComponent {
    CameraComponent(bool isPerspective)
        : camera(isPerspective)
    { }

    CameraComponent(float fov, float aspect, float near_, float far_)
        : camera(fov, aspect, near_, far_)
    { }

    CameraComponent(float left, float right, float bottom, float top, float near_, float far_)
        : camera(left, right, bottom, top, near_, far_)
    { }

public:
    Camera camera;
};

/**
 * Transform component for UI
 * Position is 2d coordinates maps to camera bounds.
 * [0;0] is a left top position of the screen and [100;100] is the right bottom position
 */
struct RectTransformComponent {
    RectTransformComponent() = default;
    RectTransformComponent(const lia::vec2& pos_, const lia::vec2& scale_ = lia::vec2(1.0f))
        : pos(pos_)
        , scale(scale_)
    { }

public:
    lia::vec2 pos;
    lia::vec2 scale { 1.0f };
};

void to_json(nlohmann::json& j, const RectTransformComponent& t);
void from_json(const nlohmann::json& j, RectTransformComponent& t);

struct TextComponent {
    TextComponent() = default;
    TextComponent(std::string_view text_, const lia::vec4& color_ = { 1.0f })
        : text(text_)
        , color(color_)
    { }

    void Show()
    {
        isVisible = true;
    }

    void Hide()
    {
        isVisible = false;
    }

public:
    std::string text;
    lia::vec4 color { 1.0f };
    bool isVisible { true };
};

void to_json(nlohmann::json& j, const TextComponent& t);
void from_json(const nlohmann::json& j, TextComponent& t);
} // namespace elv
