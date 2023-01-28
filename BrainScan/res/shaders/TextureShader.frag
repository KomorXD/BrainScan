#version 330 core

out vec4 FragColor;

in vec2 textureCoords;

uniform sampler2D u_ScanTexture;
uniform float u_Flip = 0.0;

uniform float u_MaskMin = 0.0;
uniform float u_MaskMax = 1.0;

void main()
{
	vec4 currColor = texture(u_ScanTexture, vec2(abs(u_Flip - textureCoords.x), 1.0 - textureCoords.y));

	if(currColor.r < u_MaskMin)
	{
		currColor.r = 0.0;
	}
	else if(currColor.r > u_MaskMax)
	{
		currColor.r = 1.0;
	}
	else
	{
		currColor.r = currColor.r * (u_MaskMax - u_MaskMin) + u_MaskMin;
	}

	currColor = vec4(currColor.r, currColor.r, currColor.r, 1.0);
	FragColor = currColor;
}