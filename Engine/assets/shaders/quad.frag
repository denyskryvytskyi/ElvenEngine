#version 450 core

uniform sampler2D u_screenTexture;

in vec2 v_uv;

out vec4 color;

void main()
{
	vec3 col = texture(u_screenTexture, v_uv).rgb;
	color = vec4(col, 1.0);
}