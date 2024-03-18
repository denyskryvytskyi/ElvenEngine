// Default vertex shader for the cubes (LightingSandbox)
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat4 u_InversedNormalModel;

out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{
    vec4 worldPos = vec4(a_Position, 1.0) * u_Model;
    gl_Position = worldPos * u_ViewProjection;
    v_FragPos = vec3(worldPos);
    v_Normal = mat3(transpose(u_InversedNormalModel)) * a_Normal;
}