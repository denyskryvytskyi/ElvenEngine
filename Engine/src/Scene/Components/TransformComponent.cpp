#include <json.hpp>

#include "TransformComponent.h"

#include "ComponentSerializerHelper.h"

namespace elv {
TransformComponent::TransformComponent(const lia::vec3& pos, const lia::vec3& scale, const lia::vec3& rotation)
    : m_position(pos)
    , m_scale(scale)
    , m_rotation(rotation)
{
}

void TransformComponent::SetPosition(const lia::vec3& position)
{
    m_position = position;
    m_isDirty = true;
}

void TransformComponent::SetPositionX(const float x)
{
    m_position.x = x;
    m_isDirty = true;
}

void TransformComponent::SetPositionY(const float y)
{
    m_position.y = y;
    m_isDirty = true;
}

void TransformComponent::SetPositionZ(const float z)
{
    m_position.z = z;
    m_isDirty = true;
}

void TransformComponent::SetRotation(const lia::vec3& rotation)
{
    m_rotation = rotation;
    m_isDirty = true;
}

void TransformComponent::SetScale(const lia::vec3& scale)
{
    m_scale = scale;
    m_isDirty = true;
}

void TransformComponent::Translate(const lia::vec3& translation)
{
    m_position += translation;
    m_isDirty = true;
}

void TransformComponent::TranslateX(const float translation)
{
    m_position.x += translation;
    m_isDirty = true;
}

void TransformComponent::TranslateY(const float translation)
{
    m_position.y += translation;
    m_isDirty = true;
}

void TransformComponent::TranslateZ(const float translation)
{
    m_position.z += translation;
    m_isDirty = true;
}

void TransformComponent::UpdateLocalMatrix()
{
    const lia::mat4 rotationMatrix = lia::quaternion(m_rotation).getRotationMatrix();

    m_localMatrix = lia::scale({ 1.0f }, m_scale)
        * rotationMatrix
        * lia::translate({ 1.0f }, m_position);

    m_modelMatrix = m_localMatrix;

    m_isDirty = false;
}

void TransformComponent::UpdateFromParent(const lia::mat4& parentLocalMatrix)
{
    m_modelMatrix = m_modelMatrix * parentLocalMatrix;
}

void TransformComponent::FinalizeUpdate()
{
    m_normalMatrix = lia::transpose(lia::inverse(m_modelMatrix));
}

// TODO: serialize matrices
void to_json(nlohmann::json& j, const TransformComponent& t)
{
    j["position"] = t.GetPosition();
    j["rotation"] = t.GetRotation();
    j["scale"] = t.GetScale();
}

void from_json(const nlohmann::json& j, TransformComponent& t)
{
    lia::vec3 position;
    lia::vec3 rotation;
    lia::vec3 scale;

    j.at("position").get_to(position);
    j.at("rotation").get_to(rotation);
    j.at("scale").get_to(scale);

    t.SetPosition(position);
    t.SetRotation(rotation);
    t.SetScale(scale);
}

} // namespace elv
