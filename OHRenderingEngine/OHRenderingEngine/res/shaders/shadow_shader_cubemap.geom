#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices  = 18) out;

out GS_OUT
{
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
			gl_Position = TransformationMatrix[face] * gs_out.fragPosWorld;
			EmitVertex();
		}
		EndPrimitive();
	}
}