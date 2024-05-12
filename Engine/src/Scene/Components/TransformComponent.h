#pragma once

#include <lia/mat4.h>
#include <lia/vec3.h>
#include <nlohmann/json_fwd.hpp>

namespace elv {
class TransformComponent {
public:
    TransformComponent() = default;
    TransformComponent(const lia::vec3& pos, const lia::vec3& scale = { 1.0f }, const lia::vec3& rotation = { 0.0f });

    void SetPosition(const lia::vec3& position);
    void SetPositionX(const float x);
    void SetPositionY(const float y);
    void SetPositionZ(const float z);
    // Rotation angles in radians
    void SetRotation(const lia::vec3& rotation);
    void SetScale(const lia::vec3& scale);

    void Translate(const lia::vec3& translation);
    void TranslateX(const float translation);
    void TranslateY(const float translation);
    void TranslateZ(const float translation);

    void UpdateLocalMatrix();
    void UpdateFromParent(const lia::mat4& parentLocalMatrix);
    void FinalizeUpdate();

    const lia::vec3& GetPosition() const { return m_position; }
    const lia::vec3& GetRotation() const { return m_rotation; }
    const lia::vec3& GetScale() const { return m_scale; }

    const lia::mat4& GetLocalMatrix() const { return m_localMatrix; }
    const lia::mat4& GetModelMatrix() const { return m_modelMatrix; }
    const lia::mat4& GetNormalModelMatrix() const { return m_modelMatrix; }

    bool IsDirty() const { return m_isDirty; }

private:
    bool m_isDirty { true };
    lia::vec3 m_position;
    lia::vec3 m_rotation; // Euler angles
    lia::vec3 m_scale { 1.0f };

    lia::mat4 m_localMatrix;  // local space
    lia::mat4 m_modelMatrix;  // world space
    lia::mat4 m_normalMatrix; // matrix to handle transformation for the normal vector
};

void to_json(nlohmann::json& j, const TransformComponent& t);
void from_json(const nlohmann::json& j, TransformComponent& t);

} // namespace elv
