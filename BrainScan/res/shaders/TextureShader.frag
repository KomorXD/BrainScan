#version 330 core

out vec4 FragColor;

in vec2 textureCoords;

uniform sampler2D u_ScanTexture;

void main()
{
	vec4 currColor = texture(u_ScanTexture, vec2(textureCoords.x, 1.0 - textureCoords.y));

	currColor = vec4(currColor.r, currColor.r, currColor.r, 1.0);
	FragColor = currColor;
}