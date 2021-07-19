#version 400 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
in vec3 v_TexCoords;

uniform samplerCube skybox;

void main()
{

	FragColor = texture(skybox, v_TexCoords);
	BrightColor = vec4(0,0,0,1);
}