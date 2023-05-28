#include <json.hpp>

#include "SceneComponents.h"

#include "Scene/Behavior.h"

// SpriteComponent
#include "Core/StringId.h"
#include "Events/EventManager.h"
#include "Events/TextureEvent.h"
#include "Resources/TextureManager.h"

namespace lia {
using json = nlohmann::json;

void to_json(json& j, const lia::vec4& vec)
{
    j["x"] = vec.x;
    j["y"] = vec.y;
    j["z"] = vec.z;
    j["w"] = vec.w;
}

void from_json(const nlohmann::json& j, lia::vec4& vec)
{
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
    j.at("z").get_to(vec.z);
    j.at("w").get_to(vec.w);
}

void to_json(json& j, const lia::vec3& vec)
{
    j["x"] = vec.x;
    j["y"] = vec.y;
    j["z"] = vec.z;
}

void from_json(const nlohmann::json& j, lia::vec3& vec)
{
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
    j.at("z").get_to(vec.z);
}

void to_json(json& j, const lia::vec2& vec)
{
    j["x"] = vec.x;
    j["y"] = vec.y;
}

void from_json(const nlohmann::json& j, lia::vec2& vec)
{
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
}
} // namespace lia

namespace elv {

void to_json(nlohmann::json& j, const TransformComponent& t)
{
    j["position"] = t.pos;
    j["rotation"] = t.rotation;
    j["scale"] = t.scale;
}

void from_json(const nlohmann::json& j, TransformComponent& t)
{
    j.at("position").get_to(t.pos);
    j.at("rotation").get_to(t.rotation);
    j.at("scale").get_to(t.scale);
}

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
            EL_CORE_WARN("Failed to set texture to hte Sprite component, texturePath is missed");
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

// void SriteAnimationComponent::LoadTextures(const std::string& pathToTextures)
//{
//     for (auto textureName : textures) {
//         if (!textureName.empty() && !pathToTextures.empty()) {
//             auto texturePtr = textures::Get(textureName);
//             if (!texturePtr) {
//                 textures::Load(textureName, pathToTextures);
//             }
//         } else {
//             EL_CORE_WARN("Texture name is wrong.");
//         }
//     }
// }

} // namespace elv
