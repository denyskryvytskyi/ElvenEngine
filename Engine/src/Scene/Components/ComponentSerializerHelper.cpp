#include <nlohmann/json.hpp>

#include "ComponentSerializerHelper.h"

namespace lia {
void to_json(json& j, const lia::vec4& vec)
{
    j["x"] = vec.x;
    j["y"] = vec.y;
    j["z"] = vec.z;
    j["w"] = vec.w;
}

void from_json(const json& j, lia::vec4& vec)
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

void from_json(const json& j, lia::vec3& vec)
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

void from_json(const json& j, lia::vec2& vec)
{
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
}
} // namespace lia
