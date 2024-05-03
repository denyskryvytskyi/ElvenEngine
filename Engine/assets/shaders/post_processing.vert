#version 450 core
layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec2 a_UV;

out vec2 v_UV;
out vec2 v_Pos;

void main()
{
    v_UV = a_UV;
    v_Pos = a_Pos;
    gl_Position = vec4(a_Pos, 0.0, 1.0);
}