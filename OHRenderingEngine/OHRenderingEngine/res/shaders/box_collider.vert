
#version 400 core
layout (location = 0) in vec3 aLowerBound;
layout (location = 1) in vec3 aUpperBound;

out VS_OUT
{
	vec4 v_Vertex[16];
} vs_out;

uniform mat4 view;

void main()
{
	vec3 v1 = aLowerBound;
	vec3 v2 = aUpperBound;
    vs_out.v_Vertex[0] = view * vec4(v1.xyz, 1.0f);
    vs_out.v_Vertex[1] = view * vec4(v2.x, v1.yz, 1.0f);
	vs_out.v_Vertex[2] = view * vec4(v2.x, v1.y, v2.z, 1.0f);
	vs_out.v_Vertex[3] = view * vec4(v1.xy, v2.z, 1.0f);
	vs_out.v_Vertex[4] = view * vec4(v1.xyz, 1.0f);
	vs_out.v_Vertex[5] = view * vec4(v1.x, v2.y, v1.z, 1.0f);
	vs_out.v_Vertex[6] = view * vec4(v1.x, v2.yz, 1.0f);
	vs_out.v_Vertex[7] = view * vec4(v2.xyz, 1.0f);
	vs_out.v_Vertex[8] = view * vec4(v2.xy, v1.z, 1.0f);
	vs_out.v_Vertex[9] = view * vec4(v1.x, v2.y, v1.z, 1.0f);
	vs_out.v_Vertex[10] = view * vec4(v2.xy, v1.z, 1.0f);
	vs_out.v_Vertex[11] = view * vec4(v2.x, v1.yz, 1.0f);
	vs_out.v_Vertex[12] = view * vec4(v2.x, v1.y, v2.z, 1.0f);
	vs_out.v_Vertex[13] = view * vec4(v2.xyz, 1.0f);
	vs_out.v_Vertex[14] = view * vec4(v1.x, v2.yz, 1.0f);
	vs_out.v_Vertex[15] = view * vec4(v1.xy, v2.z, 1.0f);
	gl_Position = vec4(0,0,0,0);
}