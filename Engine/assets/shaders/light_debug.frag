// Fragment shader for the light objects (LightingSandbox)
#version 450 core

struct LightColor {
    vec3 ambient;
    vec3 diffuse;
};
uniform LightColor u_Color;

out vec4 FragColor;

void main()
{
    vec3 result = u_Color.ambient + u_Color.diffuse;
    FragColor = vec4(result, 1.0);
}