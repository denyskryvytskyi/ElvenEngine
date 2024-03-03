#pragma once
namespace elv {
struct PointLight {
    lia::vec3 position { 0.0f, 0.8f, 1.2f };

    lia::vec3 ambient { 0.2f, 0.2f, 0.2f };
    lia::vec3 diffuse { 0.4f, 0.4f, 0.4f };
    lia::vec3 specular { 1.0f, 1.0f, 1.0f };

    // distance 50
    float constant { 1.0f };
    float linear { 0.09f };
    float quadratic { 0.032f };
};

struct DirectionalLight {
    lia::vec3 direction { -0.2f, -1.0f, -0.3f };

    lia::vec3 ambient { 0.2f, 0.2f, 0.2f };
    lia::vec3 diffuse { 0.4f, 0.4f, 0.4f };
    lia::vec3 specular { 1.0f, 1.0f, 1.0f };
};

struct SpotLight {
    lia::vec3 position;
    lia::vec3 direction;

    lia::vec3 ambient { 0.2f, 0.2f, 0.2f };
    lia::vec3 diffuse { 0.4f, 0.4f, 0.4f };
    lia::vec3 specular { 1.0f, 1.0f, 1.0f };

    float cutOff { 12.5f };
    float outerCutOff { 17.5f };
    // distance 50
    float constant { 1.0f };
    float linear { 0.09f };
    float quadratic { 0.032f };
};

} // namespace elv
