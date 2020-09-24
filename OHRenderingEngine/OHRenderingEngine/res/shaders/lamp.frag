#version 400 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
in vec3 v_Color;



void main()
{
	FragColor = vec4(v_Color, 1.0f);
	BrightColor = vec4(v_Color, 1.0f);
}