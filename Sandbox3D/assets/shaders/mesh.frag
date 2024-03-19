// Fragment shader for the mesh
#version 450 core

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_emission;
    sampler2D texture_transparency;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 emissionColor;
    float shininess;
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

vec3 CalcDirLight(DirLight light, vec3 diffuseMap, float specularMap, vec3 emissionMap, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 diffuseMap, float specularMap, vec3 emissionMap, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light,vec3 diffuseMap, float specularMap, vec3 emissionMap, vec3 normal, vec3 viewDir);

void main()
{
    // check base texture alpha
    vec4 diffuseMapWithAlpha = texture(u_Material.texture_diffuse, v_UV).rgba;
    if (diffuseMapWithAlpha.a < 0.5)
    {
        discard;
    }

    // maps
    vec3 diffuseMap = diffuseMapWithAlpha.rgb * u_Material.diffuseColor;

    float specularMap = texture(u_Material.texture_specular, v_UV).r;

    vec3 emissionMap = texture(u_Material.texture_emission, v_UV).rgb * u_Material.emissionColor;

    // output color
    vec3 result = vec3(0.0);

    vec3 normal = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);

    // lights calculation
    if (u_DirLightEnabled)
    {
        result += CalcDirLight(u_DirLight, diffuseMap, specularMap, emissionMap, normal, viewDir);
    }

    if (u_PointLightEnabled)
    {
        for (int i = 0; i < NR_POINT_LIGHTS; ++i)
            result += CalcPointLight(u_PointLights[i], diffuseMap, specularMap, emissionMap, normal, viewDir);
    }

    if (u_SpotLightEnabled)
    {
        result += CalcSpotLight(u_SpotLight, diffuseMap, specularMap, emissionMap, normal, viewDir);
    }

    float alpha = texture(u_Material.texture_transparency, v_UV).r * diffuseMapWithAlpha.a;

    FragColor = vec4(result, alpha);
}

vec3 CalcDirLight(DirLight light, vec3 diffuseMap, float specularMap, vec3 emissionMap, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    // ambient
    vec3 ambient = light.ambient * diffuseMap * u_Material.ambientColor;

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * diffuseMap;

    // specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = light.specular * spec * specularMap * u_Material.specularColor;

    // emission
    vec3 emission = emissionMap;

    return ambient + diffuse + specular + emission;
}

vec3 CalcPointLight(PointLight light, vec3 diffuseMap, float specularMap, vec3 emissionMap, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // ambient
    vec3 ambient = light.ambient * diffuseMap * u_Material.ambientColor;

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * diffuseMap;

    // specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = light.specular * spec * specularMap * u_Material.specularColor;

    // emission
    vec3 emission = emissionMap;

    // attenuation
    float distance = length(light.position - v_FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular + emission);
}

vec3 CalcSpotLight(SpotLight light, vec3 diffuseMap, float specularMap, vec3 emissionMap, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // ambient
    vec3 ambient = light.ambient * diffuseMap * u_Material.ambientColor;

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * diffuseMap;

    // specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = light.specular * spec * specularMap * u_Material.specularColor;

    // emission
    vec3 emission = emissionMap;

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

    return ambient + diffuse + specular + emission;
}
