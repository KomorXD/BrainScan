#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 100) out;

in VS_OUT
{
	vec3  outColor;
	float radius;
} gs_in[];

out vec3 color;

void main()
{
	color = gs_in[0].outColor;

	float radius  = gs_in[0].radius / 2.0;
	vec4 position = gl_in[0].gl_Position;

	for(float i = 0.0; i < 6.28; i += 0.2)
	{
		gl_Position = position;
		EmitVertex();
		
		gl_Position = vec4(position.x + radius * cos(i), position.y + radius * sin(i), position.zw);
		EmitVertex();
		
		gl_Position = vec4(position.x + radius * cos(i + 0.2), position.y + radius * sin(i + 0.2), position.zw);
		EmitVertex();

		EndPrimitive();
	}
}