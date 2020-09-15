#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices  = 3) out;

in VS_OUT
{
	vec3 v_NormalDir;
	vec2 v_TexCoords;
	vec3 fragPos;
} gs_in[];

out GS_OUT
{
	vec3 v_NormalDir;
	vec2 v_TexCoords;
	vec3 fragPos;
} gs_out;

uniform float time;
//uniform mat4 projection;

void main()
{
	for(int index = 0; index < 3; index++)
	{
		gs_out.v_NormalDir = gs_in[index].v_NormalDir;
		gs_out.v_TexCoords = gs_in[index].v_TexCoords;
		gs_out.fragPos = gs_in[index].fragPos;
		gl_Position =  ( gl_in[index].gl_Position + vec4(gs_in[index].v_NormalDir*time*0.0f,0.0f));
		EmitVertex();
	}
	EndPrimitive();
}