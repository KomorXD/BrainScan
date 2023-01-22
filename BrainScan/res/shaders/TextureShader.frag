#version 330 core

out vec4 FragColor;

in vec2 textureCoords;

struct Point
{
	float x;
	float y;
};

uniform sampler2D u_ScanTexture;
uniform Point u_BrushPoints[256];
uniform int u_BrushPointsSize;

void main()
{
	vec4 currColor = texture(u_ScanTexture, vec2(textureCoords.x, 1.0 - textureCoords.y));

	currColor = vec4(currColor.r, currColor.r, currColor.r, 1.0);

	for(int i = 0; i < u_BrushPointsSize; i++)
	{
		vec2 pointPos = vec2(u_BrushPoints[i].x, u_BrushPoints[i].y);

		if(distance(textureCoords, pointPos) < 0.01)
		{
			currColor = vec4(1.0, 0.0, 0.0, 1.0);
		}
	}

	FragColor = currColor;
}