#version 450 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_UV;
layout(location = 3) in float a_TextureUnit;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_UV;
flat out int v_TextureUnit;

void main()
{
    gl_Position = u_ViewProjection * a_Position;
    v_UV = a_UV;
    v_Color = a_Color;
    v_TextureUnit = int(a_TextureUnit);
}