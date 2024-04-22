#version 450 core

layout (binding = 0) uniform sampler2D u_Textures[32];

in vec4 v_Color;
in vec2 v_UV;
flat in int v_TextureUnit;

out vec4 color;

void main()
{
	color = texture(u_Textures[v_TextureUnit], v_UV) * v_Color;
}