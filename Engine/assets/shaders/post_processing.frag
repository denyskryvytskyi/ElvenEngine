#version 450 core

layout (binding = 0) uniform sampler2D u_ScreenTexture;
layout (binding = 1) uniform sampler2D u_Mask;

in vec2 v_UV;
in vec2 v_Pos;

out vec4 FragColor;

void main()
{
	vec3 finalColor = vec3(0.0);
 	vec3 originalColor = texture(u_ScreenTexture, v_UV).rgb;

	float maskValue = texture(u_Mask, v_UV).r;

	if (maskValue > 0.0)
	{
		float offset = 1.0 / 150.0;
	
		vec2 box[9] = vec2[](
			vec2(-offset, offset), vec2(0.0, offset), vec2(offset, offset),
			vec2(-offset, 0.0), vec2(0.0f, 0.0), vec2(offset,  0.0),
			vec2(-offset, -offset),	vec2(0.0, -offset), vec2(offset, -offset)
		);

		float kernel[9] = float[](
			0.0625, 0.125, 0.0625,
			0.125, 0.25, 0.125,
			0.0625, 0.125, 0.0625  
		);

		vec3 sampleTex[9];
		for(int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(texture(u_ScreenTexture, v_UV + box[i]));
		}

		vec3 blurredColor = vec3(0.0);
		for(int i = 0; i < 9; i++)
			blurredColor += sampleTex[i] * kernel[i];

		finalColor = mix(originalColor, blurredColor, maskValue);
	}
	else
	{
		finalColor = originalColor;
	}

	FragColor = vec4(finalColor, 1.0);
}