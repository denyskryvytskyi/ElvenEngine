#pragma once

constexpr int kEnvironments = 5;
constexpr int kPointLightsAmount = 4;

static const lia::vec3 kPointLightPositions[] = {
    lia::vec3(0.0f, 0.8f, 1.2f),
    lia::vec3(2.3f, -3.3f, -4.0f),
    lia::vec3(-4.0f, 2.0f, -12.0f),
    lia::vec3(0.0f, 0.0f, -3.0f)
};

static const lia::vec3 kCubePositions[] = {
    lia::vec3(0.0f, 0.75f, 0.0f),
    lia::vec3(2.0f, 5.0f, -15.0f),
    lia::vec3(-1.5f, -2.2f, -2.5f),
    lia::vec3(-3.8f, -2.0f, -12.3f),
    lia::vec3(2.4f, -0.4f, -3.5f),
    lia::vec3(-1.7f, 3.0f, -7.5f),
    lia::vec3(1.3f, -2.0f, -2.5f),
    lia::vec3(1.5f, 2.0f, -2.5f),
    lia::vec3(1.5f, 0.2f, -1.5f),
    lia::vec3(-1.3f, 1.0f, -1.5f)
};

struct SpotLightEnvironmentSetting {
    lia::vec3 ambient;
    lia::vec3 diffuse;
    lia::vec3 specular;
};

struct DirectionalLightEnvironmentSetting {
    lia::vec3 direction;
    lia::vec3 ambient;
    lia::vec3 diffuse;
    lia::vec3 specular;
};

struct EnvironmentMaterials {
    lia::vec4 clearColor;

    DirectionalLightEnvironmentSetting dirLight;
    SpotLightEnvironmentSetting spotLight;

    lia::vec3 pointLightColors[kPointLightsAmount];
};

static const EnvironmentMaterials kEnvironmenMaterials[kEnvironments] = {
    // ============== DEFAULT ================
    {

        // clear color
        { 0.3f, 0.5f, 0.8f, 1.0f },
        // dir light
        {
            { -0.2f, -1.0f, -0.3f }, // direction
            { 0.7f, 0.7f, 0.7f },    // ambient
            { 0.7f, 0.7f, 0.7f },    // deffuse
            { 0.7f, 0.7f, 0.7f },    // specular
        },
        // spot light
        {
            { 0.0f, 0.0f, 0.0f }, // ambient
            { 0.7f, 0.7f, 0.7f }, // deffuse
            { 0.7f, 0.7f, 0.7f }, // specular
        },
        // point lights
        {
            { 1.0f, 0.5f, 0.0f },
            { 0.0f, 1.0f, 0.5f },
            { 0.0f, 0.5f, 1.0f },
            { 0.5f, 1.0f, 0.0f } } },
    // ============== DESERT ================
    {

        // clear color
        { 0.75f, 0.52f, 0.3f, 1.0f },
        // dir light
        {
            { -0.2f, -1.0f, -0.3f }, // direction
            { 0.3f, 0.24f, 0.14f },  // ambient
            { 0.7f, 0.42f, 0.26f },  // deffuse
            { 0.5f, 0.5f, 0.5f },    // specular
        },
        // spot light
        {
            { 0.0f, 0.0f, 0.0f }, // ambient
            { 0.8f, 0.8f, 0.0f }, // deffuse
            { 0.8f, 0.8f, 0.0f }, // specular
        },
        // point lights
        {
            { 1.0f, 0.6f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
            { 1.0f, 1.0f, 0.0f },
            { 0.2f, 0.2f, 1.0f } } },
    // ============== FACTORY ================
    {

        // clear color
        { 0.1f, 0.1f, 0.1f, 1.0f },
        // dir light
        {
            { -0.2f, -1.0f, -0.3f }, // direction
            { 0.05f, 0.05f, 0.1f },  // ambient
            { 0.2f, 0.2f, 0.7f },    // deffuse
            { 0.7f, 0.7f, 0.7f },    // specular
        },
        // spot light
        {
            { 0.0f, 0.0f, 0.0f }, // ambient
            { 1.0f, 1.0f, 1.0f }, // deffuse
            { 1.0f, 1.0f, 1.0f }, // specular
        },
        // point lights
        {
            { 0.2f, 0.2f, 0.6f },
            { 0.3f, 0.3f, 0.7f },
            { 0.0f, 0.0f, 0.3f },
            { 0.4f, 0.4f, 0.4f } } },
    // ============== HORROR ================
    {

        // clear color
        { 0.0f, 0.0f, 0.0f, 1.0f },
        // dir light
        {
            { -0.2f, -1.0f, -0.3f }, // direction
            { 0.0f, 0.0f, 0.0f },    // ambient
            { 0.05f, 0.05f, 0.05f }, // deffuse
            { 0.2f, 0.2f, 0.2f },    // specular
        },
        // spot light
        {
            { 0.0f, 0.0f, 0.0f }, // ambient
            { 1.0f, 1.0f, 1.0f }, // deffuse
            { 1.0f, 1.0f, 1.0f }, // specular
        },
        // point lights
        {

            { 0.1f, 0.1f, 0.1f },
            { 0.1f, 0.1f, 0.1f },
            { 0.1f, 0.1f, 0.1f },
            { 0.3f, 0.1f, 0.1f } } },
    // ============== BIOCHEMICAL LAB ================
    {

        // clear color
        { 0.9f, 0.9f, 0.9f, 1.0f },
        // dir light
        {
            { -0.2f, -1.0f, -0.3f }, // direction
            { 0.5f, 0.5f, 0.5f },    // ambient
            { 1.0f, 1.0f, 1.0f },    // deffuse
            { 1.0f, 1.0f, 1.0f },    // specular
        },
        // spot light
        {
            { 0.0f, 0.0f, 0.0f }, // ambient
            { 0.0f, 1.0f, 0.0f }, // deffuse
            { 0.0f, 1.0f, 0.0f }, // specular
        },
        // point lights
        {
            { 0.4f, 0.7f, 0.1f },
            { 0.4f, 0.7f, 0.1f },
            { 0.4f, 0.7f, 0.1f },
            { 0.4f, 0.7f, 0.1f } } }
};
