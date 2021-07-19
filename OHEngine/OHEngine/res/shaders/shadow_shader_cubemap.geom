#version 400 core
layout (triangles) in;
layout (triangle_strip, max_vertices  = 18) out;

in VS_OUT
{
	vec2 v_TexCoords;
} gs_in[];

out GS_OUT
{
	vec2 v_TexCoords;
	vec4 fragPosWorld;
} gs_out;

uniform mat4 TransformationMatrix[6];

void main()
{
	for(int face = 0; face < 6; face++)
	{
		gl_Layer = face;
		for(int i = 0; i < 3; i++)
		{
			gs_out.fragPosWorld = gl_in[i].gl_Position;
			gs_out.v_TexCoords = gs_in[i].v_TexCoords;
			gl_Position = TransformationMatrix[face] * gs_out.fragPosWorld;
			EmitVertex();
		}
		EndPrimitive();
	}
}