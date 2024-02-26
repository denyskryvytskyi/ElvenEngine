// default fragment shader with texture (CubesSandbox)
#version 450 core

uniform vec4 u_Color;
layout (binding = 1) uniform sampler2D u_Texture;

in vec2 v_UV;
out vec4 color;

void main()
{
	color = texture(u_Texture, v_UV);
}