#include <json.hpp>

#include "SceneComponents.h"

#include "Scene/Behavior.h"

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
}

void from_json(const nlohmann::json& j, SpriteComponent& t)
{
    j.at("texture_name").get_to<std::string>(t.textureName);
}
} // namespace elv
