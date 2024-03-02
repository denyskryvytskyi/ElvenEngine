// Fragment shader for the cube with texture (LightingSandbox)
#version 450 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
    bool enableEmission;
}; 
uniform Material u_Material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Light u_Light;

uniform vec3 u_ViewPos;

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_UV;

out vec4 FragColor;

void main()
{
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_Light.position - v_FragPos);

    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // ambient
    vec3 diffuseMap = texture(u_Material.diffuse, v_UV).rgb;
    vec3 ambient = u_Light.ambient * diffuseMap;

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, v_UV).rgb;

    // specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specularMap = texture(u_Material.specular, v_UV).rgb;
    vec3 specular = u_Light.specular * spec * specularMap; 

    // emission
    vec3 emission = vec3(0.0f);

    if (u_Material.enableEmission && specularMap == vec3(0.0f))
    {
        emission = texture(u_Material.emission, v_UV).rgb;
    }

    // attenuation
    float distance = length(u_Light.position - v_FragPos);
    float attenuation = 1.0 / (u_Light.constant + u_Light.linear * distance + u_Light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // final
    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}  