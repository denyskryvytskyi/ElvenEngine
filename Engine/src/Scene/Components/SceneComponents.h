#pragma once

#include <json_fwd.hpp>

#include "Scene/Behavior.h"

#include "Renderer/Camera.h"

namespace elv {
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
    void SetTexture(const std::string& texture_name, const std::string& texture_path)
    {
        textureName = texture_name;
        texturePath = texture_path;
        LoadTexture();
    }

    void SetColor(const lia::vec4& color_)
    {
        color = color_;
    }

    // Load texture if it has textureName and texturePath
    // Actually is used only in case of scene deserialization
    void LoadTexture();

public:
    std::string textureName;
    std::string texturePath;
    SharedPtr<Texture2D> texture { nullptr };
    lia::vec4 color { 1.0f };
};

void to_json(nlohmann::json& j, const SpriteComponent& t);
void from_json(const nlohmann::json& j, SpriteComponent& t);

// TODO: spritesheet animation
// struct SriteAnimation {
//    SriteAnimation(int frames_count, float time_per_frame)
//        : framesCount(frames_count)
//        , timePerFrame(time_per_frame)
//    { }
//
//    void AddTextures(std::string_view sprite_sheet_name, const std::string_view sprite_sheet_path)
//    {
//        /*textures = std::move(textures_);
//        LoadTextures(pathToTextures);*/
//    }
//
//    void Update(float elapsed)
//    {
//
//        elapsedTime += elapsed;
//    }
//
// private:
//    void LoadTextures(const std::string& pathToTextures);
//
// public:
//    int framesCount { 0 };
//    float timePerFrame { 0.0f };  // time while one frame is active
//    int currentTextureName { 0 }; // name of the current frame texture
//    float elapsedTime { 0.0f };   // internal usage
//    std::vector<std::string> textures;
//};

struct BehaviorComponent {
public:
    BehaviorComponent() = default;
    BehaviorComponent(BehaviorComponent&&) = default;
    ~BehaviorComponent()
    {
        if (behavior) {
            behavior->OnDestroy();
        }
    }

    BehaviorComponent& operator=(BehaviorComponent&& other) = default;

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
    bool isEnabled { true };
    std::unique_ptr<ecs::IBehavior> behavior;
    std::function<std::unique_ptr<ecs::IBehavior>()> instantiateBehavior;
};

struct CameraComponent {
    CameraComponent()
        : camera(false)
    { }

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
    TextComponent(std::string_view text_, const lia::vec4& color_ = { 1.0f }, std::string_view font_name = "arial")
        : text(text_)
        , color(color_)
        , fontName(font_name)
    { }

    void Show()
    {
        isVisible = true;
    }

    void Hide()
    {
        isVisible = false;
    }

    void SetFontName(std::string_view font_name)
    {
        fontName = font_name;
    }

public:
    std::string text;
    lia::vec4 color { 1.0f };
    bool isVisible { true };
    std::string fontName { "arial" };
};

void to_json(nlohmann::json& j, const TextComponent& t);
void from_json(const nlohmann::json& j, TextComponent& t);

struct AABBComponent {
    lia::vec2 size { 0.0f };
};

struct TagComponent {
    std::string tag;
};

struct SoundComponent {

    ~SoundComponent();

    void Play();
    void Pause();
    void Stop();

    void UpdateVolume();

    bool isLooped { true };
    float volume { 0.5f };
    std::string soundName;
};

} // namespace elv
