// Vertex shader for the light objects (LightingSandbox)
#version 450 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

void main()
{
    gl_Position = vec4(a_Position, 1.0) * u_Model * u_ViewProjection;
}