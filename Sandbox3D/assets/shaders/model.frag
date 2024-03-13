#version 450 core
out vec4 FragColor;

in vec2 v_UV;

uniform sampler2D texture_diffuse;

void main()
{
    FragColor = vec4(1.0);
}