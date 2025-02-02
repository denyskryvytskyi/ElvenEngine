// Vertex shader for the cube with texture (LightingSandbox)
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat4 u_NormalModel;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_UV;

void main()
{
    vec4 worldPos = u_Model * vec4(a_Position, 1.0);
    gl_Position = u_ViewProjection * worldPos;
    v_FragPos = vec3(worldPos);
    v_Normal = mat3(u_NormalModel) * a_Normal;
    v_UV = a_UV;
}