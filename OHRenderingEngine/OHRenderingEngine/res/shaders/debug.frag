#version 330 core

out vec4 FragColor;

in GS_OUT
{
	vec3 v_NormalDir;
} fs_in;


void main()
{	
	FragColor = vec4(1.0f,1.0f,0.0f,1.0f);
}
