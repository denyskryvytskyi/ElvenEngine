#version 450 core

uniform sampler2D u_texture;
uniform vec4 textColor;
in vec2 v_uv;

out vec4 color;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_texture, v_uv).r);
    color = textColor * sampled;
}