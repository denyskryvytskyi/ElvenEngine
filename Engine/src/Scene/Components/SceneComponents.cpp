#include <json.hpp>

#include "SceneComponents.h"

#include "ComponentSerializerHelper.h"

#include "Scene/Behavior.h"

// SpriteComponent
#include "Core/StringId.h"
#include "Events/EventManager.h"
#include "Events/TextureEvent.h"
#include "Resources/TextureManager.h"

namespace elv {
void to_json(nlohmann::json& j, const QuadComponent& t)
{
    j["color"] = t.color;
}

void from_json(const nlohmann::json& j, QuadComponent& t)
{
    j.at("color").get_to(t.color);
}

void to_json(nlohmann::json& j, const SpriteComponent& t)
{
    j["texture_name"] = t.textureName;
    j["texture_path"] = t.texturePath;
}

void from_json(const nlohmann::json& j, SpriteComponent& t)
{
    j.at("texture_name").get_to<std::string>(t.textureName);
    j.at("texture_path").get_to<std::string>(t.texturePath);
}

void SpriteComponent::LoadTexture()
{
    if (!textureName.empty()) {
        auto texturePtr = textures::Get(textureName);
        if (texturePtr) {
            texture = texturePtr;
        } else if (texturePath.empty()) {
            EL_CORE_WARN("Failed to set texture to the Sprite component, texturePath is missed");
        } else {
            textures::Load(textureName, texturePath);

            events::Subscribe<events::TextureLoadedEvent>([&](const events::TextureLoadedEvent& e) {
                if (e.textureName == textureName) {
                    texture = textures::Get(textureName);
                }
            },
                                                          string_id(textureName));
        }
    } else {
        EL_CORE_WARN("Texture loading is failed. Please call SetTexture function first or set members using ctor.")
    }
}

void to_json(nlohmann::json& j, const RectTransformComponent& t)
{
    j["pos"] = t.pos;
    j["scale"] = t.scale;
}
void from_json(const nlohmann::json& j, RectTransformComponent& t)
{
    j.at("position").get_to(t.pos);
    j.at("scale").get_to(t.scale);
}

void to_json(nlohmann::json& j, const TextComponent& t)
{
    j["text"] = t.text;
    j["color"] = t.color;
    j["is_visible"] = t.isVisible;
}
void from_json(const nlohmann::json& j, TextComponent& t)
{
    j.at("text").get_to(t.text);
    j.at("color").get_to(t.color);
    j.at("is_visible").get_to(t.isVisible);
}
} // namespace elv
