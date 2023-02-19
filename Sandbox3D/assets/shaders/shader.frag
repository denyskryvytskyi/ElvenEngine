#version 450 core

uniform vec4 u_Color;
layout (binding = 1) uniform sampler2D u_Texture;

out vec4 color;

in vec2 v_UV;

void main()
{
	color = texture(u_Texture, v_UV);
}