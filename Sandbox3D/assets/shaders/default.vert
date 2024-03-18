// default vertex shader (CubesSandbox)
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_UV;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out vec2 v_UV;

void main()
{
    gl_Position = vec4(a_Position, 1.0) * u_Model * u_ViewProjection;
    v_UV = a_UV;
}