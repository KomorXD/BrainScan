#version 330 core

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

float thickness = 0.05;

void main()
{
	float radius = thickness / 2.0;

	vec4 p1 = gl_in[0].gl_Position;
	vec4 p2 = gl_in[1].gl_Position;

	vec2 dir	= normalize(p2.xy - p1.xy);
	vec2 normal = vec2(dir.y, -dir.x);

	vec4 offset = vec4(normal * radius, 0.0, 0.0);

	vec4 coords[4];
	
	coords[0] = p1 - offset;
	coords[1] = p2 - offset;
	coords[2] = p1 + offset;
	coords[3] = p2 + offset;

	for(int i = 0; i < 4; ++i)
	{
		gl_Position = coords[i];

		EmitVertex();
	}

	EndPrimitive();
}