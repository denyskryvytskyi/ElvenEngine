#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TextureCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out vec2 v_TextureCoords;

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0f);
    v_TextureCoords = vec2(a_TextureCoords.x, a_TextureCoords.y);
}