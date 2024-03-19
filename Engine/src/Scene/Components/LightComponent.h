#pragma once

#include <json_fwd.hpp>

namespace elv {
struct PointLightComponent {
    lia::vec3 ambient;
    lia::vec3 diffuse;
    lia::vec3 specular;

    // distance 50
    float constant { 1.0f };
    float linear { 0.09f };
    float quadratic { 0.032f };
};

struct DirectionalLightComponent {
    lia::vec3 ambient;
    lia::vec3 diffuse;
    lia::vec3 specular;
};

struct SpotLightComponent {
    lia::vec3 ambient;
    lia::vec3 diffuse;
    lia::vec3 specular;

    float cutOff { 12.5f };
    float outerCutOff { 17.5f };
    // distance 50
    float constant { 1.0f };
    float linear { 0.09f };
    float quadratic { 0.032f };
};

} // namespace elv
