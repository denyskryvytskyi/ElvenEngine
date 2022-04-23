#version 450 core

uniform vec4 u_Color;

out vec4 color;

in vec3 v_Position;

void main()
{
	color = u_Color;
}