// Default fragment shader for the cubes (LightingSandbox)
#version 450 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material u_Material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light u_Light;

uniform vec3 u_ViewPos;

in vec3 v_Normal;
in vec3 v_FragPos;

out vec4 FragColor;

void main()
{
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_Light.position - v_FragPos);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    // ambient
    vec3 ambient = u_Material.ambient * u_Light.ambient;

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = u_Light.diffuse * (diff * u_Material.diffuse);

    // specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = (u_Material.specular * spec) * u_Light.specular; 

    // final
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}  