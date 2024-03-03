// Fragment shader for the cube with texture (LightingSandbox)
// with support of directional, point, and spotlight
#version 450 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
    bool enableEmission;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
    float cutOff; // cos of the angle
    float outerCutOff;
};

#define NR_POINT_LIGHTS 4

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform bool u_DirLightEnabled;
uniform bool u_PointLightEnabled;
uniform bool u_SpotLightEnabled;
uniform DirLight u_DirLight;
uniform PointLight u_PointLights[NR_POINT_LIGHTS];
uniform SpotLight u_SpotLight;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_UV;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

void main()
{
    // output color
    vec3 result = vec3(0.0);

    vec3 normal = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    // lights calculation
    if (u_DirLightEnabled)
    {
        result += CalcDirLight(u_DirLight, normal, viewDir);
    }

    if (u_PointLightEnabled)
    {
        for (int i = 0; i < NR_POINT_LIGHTS; ++i)
            result += CalcPointLight(u_PointLights[i], normal, viewDir);
    }

    if (u_SpotLightEnabled)
    {
        result += CalcSpotLight(u_SpotLight, normal, viewDir);
    }

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    // ambient
    vec3 diffuseMap = texture(u_Material.diffuse, v_UV).rgb;
    vec3 ambient = light.ambient * diffuseMap;

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * texture(u_Material.diffuse, v_UV).rgb;

    // specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specularMap = texture(u_Material.specular, v_UV).rgb;
    vec3 specular = light.specular * spec * specularMap; 

    // emission
    vec3 emission = vec3(0.0f);

    if (u_Material.enableEmission && specularMap == vec3(0.0f))
    {
        emission = texture(u_Material.emission, v_UV).rgb;
    }

    return (ambient + diffuse + specular + emission);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // ambient
    vec3 diffuseMap = texture(u_Material.diffuse, v_UV).rgb;
    vec3 ambient = light.ambient * diffuseMap;

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * texture(u_Material.diffuse, v_UV).rgb;

    // specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specularMap = texture(u_Material.specular, v_UV).rgb;
    vec3 specular = light.specular * spec * specularMap; 

    // emission
    vec3 emission = vec3(0.0f);

    if (u_Material.enableEmission && specularMap == vec3(0.0f))
    {
        emission = texture(u_Material.emission, v_UV).rgb;
    }

    // attenuation
    float distance = length(light.position - v_FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular + emission);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // ambient
    vec3 diffuseMap = texture(u_Material.diffuse, v_UV).rgb;
    vec3 ambient = light.ambient * diffuseMap;

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * texture(u_Material.diffuse, v_UV).rgb;

    // specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specularMap = texture(u_Material.specular, v_UV).rgb;
    vec3 specular = light.specular * spec * specularMap; 

    // emission
    vec3 emission = vec3(0.0f);

    if (u_Material.enableEmission && specularMap == vec3(0.0f))
    {
        emission = texture(u_Material.emission, v_UV).rgb;
    }

    // spotlight with soft edges
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(light.position - v_FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular + emission);
}
