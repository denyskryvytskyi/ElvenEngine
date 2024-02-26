// Default fragment shader for the cubes (LightingSandbox)
#version 450 core

uniform vec3 u_ObjectColor;
uniform vec3 u_LightColor;

out vec4 FragColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_LightColor;

    vec3 result = ambient * u_ObjectColor;
    FragColor = vec4(result, 1.0);
}  