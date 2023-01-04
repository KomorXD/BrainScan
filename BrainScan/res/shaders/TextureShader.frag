#version 330 core

out vec4 FragColor;

in vec2 textureCoords;

uniform sampler2D u_ScanTexture;

void main()
{
	FragColor = 1.0 - texture(u_ScanTexture, textureCoords);
}