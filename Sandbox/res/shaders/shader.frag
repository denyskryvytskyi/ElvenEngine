#version 330 core

out vec4 color;

in vec3 v_Position;
in vec4 v_Color;

void main()
{
	color = v_Color;
}