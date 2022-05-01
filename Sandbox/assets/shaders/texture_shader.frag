#version 450 core

uniform vec4 u_Color;
uniform sampler2D u_Texture;

in vec2 v_TextureCoords;

out vec4 color;

void main()
{
	color = texture(u_Texture, v_TextureCoords) * u_Color;
}