#version 450 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

out vec2 v_uv;

void main()
{
    v_uv = uv;
    gl_Position = vec4(pos, 0.0, 1.0);
}