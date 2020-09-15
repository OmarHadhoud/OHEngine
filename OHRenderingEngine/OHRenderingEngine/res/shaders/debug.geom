#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices  = 6) out;

in VS_OUT
{
	vec3 v_NormalDir;
} gs_in[];


uniform mat4 projection;
float normal_length = 0.4f;
void main()
{
	for(int index = 0; index < 3; index++)
	{
		gl_Position = projection * ( gl_in[index].gl_Position);
		EmitVertex();
		gl_Position = projection * ( gl_in[index].gl_Position + normal_length*normalize(vec4(gs_in[index].v_NormalDir,0.0f)));
		EmitVertex();
		EndPrimitive();
	}
}