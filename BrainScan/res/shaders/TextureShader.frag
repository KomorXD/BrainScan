#version 330 core

out vec4 FragColor;

in vec2 textureCoords;

uniform sampler2D u_ScanTexture;
uniform float u_Flip = 0.0;

void main()
{
	vec4 currColor = texture(u_ScanTexture, vec2(abs(u_Flip - textureCoords.x), 1.0 - textureCoords.y));

	currColor = vec4(currColor.r, currColor.r, currColor.r, 1.0);
	FragColor = currColor;
}