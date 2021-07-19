#version 400 core
layout (points) in;
layout (line_strip, max_vertices  = 16) out;

in VS_OUT
{
	vec4 v_Vertex[16];
} gs_in[];


uniform mat4 projection;
void main()
{
	for(int i = 0; i < 16; i++)
	{
		gl_Position = projection * gs_in[0].v_Vertex[i];
		EmitVertex();
	}
	EndPrimitive();
	
}