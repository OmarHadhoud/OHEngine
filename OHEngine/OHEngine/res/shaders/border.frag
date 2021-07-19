#version 400 core

out vec4 FragColor;
uniform vec3 BorderColor;



void main()
{

	FragColor = vec4(BorderColor, 1.0f);

}