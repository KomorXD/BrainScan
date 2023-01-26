#version 330 core

layout(location = 0) in vec2  a_Pos;
layout(location = 1) in vec3  a_Color;
layout(location = 2) in float a_Radius;

out VS_OUT
{
	vec3  outColor;
	float radius;
} vs_out;

void main()
{
	gl_PointSize = 50.0;
	gl_Position = vec4(a_Pos.x * 2.0 - 1.0, a_Pos.y * 2.0 - 1.0, 0.0, 1.0);
	
	vs_out.outColor = a_Color;
	vs_out.radius   = a_Radius / 50.0;
}