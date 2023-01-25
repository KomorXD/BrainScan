#version 330 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec3 a_Color;

out vec4 outColor;

void main()
{
	gl_Position = vec4(a_Pos.x * 2.0 - 1.0, a_Pos.y * 2.0 - 1.0, 0.0, 1.0);
	outColor = vec4(a_Color, 1.0);
}