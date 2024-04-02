#pragma once

#include <json_fwd.hpp>
#include <lia/mat4.h>
#include <lia/vec3.h>

namespace elv {
class TransformComponent {
public:
    TransformComponent() = default;
    TransformComponent(const lia::vec3& pos, const lia::vec3& scale = { 1.0f }, const lia::vec3& rotation = { 0.0f });

    void UpdateLocalMatrix();

public:
    bool isDirty { true };
    lia::vec3 pos;
    lia::vec3 rotation;
    lia::vec3 scale { 1.0f };

    lia::mat4 localMatrix;
    lia::mat4 modelMatrix;  // world space
    lia::mat4 normalMatrix; // matrix to handle transformation for the normal vector
};
void to_json(nlohmann::json& j, const TransformComponent& t);
void from_json(const nlohmann::json& j, TransformComponent& t);

} // namespace elv
