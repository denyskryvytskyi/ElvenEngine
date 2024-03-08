#include <json.hpp>

#include "TransformComponent.h"

#include "ComponentSerializerHelper.h"

namespace elv {
TransformComponent::TransformComponent(const lia::vec3& pos, const lia::vec3& scale, const lia::vec3& rotation)
    : pos(pos)
    , scale(scale)
    , rotation(rotation)
{
    UpdateModelMatrix();
}

void TransformComponent::UpdateModelMatrix()
{
    modelMatrix = lia::scale({ 1.0f }, lia::vec3(scale.x, scale.y, scale.z))
        * lia::rotateX({ 1.0f }, lia::radians(rotation.x))
        * lia::rotateY({ 1.0f }, lia::radians(rotation.y))
        * lia::rotateZ({ 1.0f }, lia::radians(rotation.z))
        * lia::translate({ 1.0f }, pos);
}

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

} // namespace elv
