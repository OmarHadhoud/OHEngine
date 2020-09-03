#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
out vec3 v_Color;
out vec2 v_TexCoords;

void main()
{
    gl_Position = vec4(aPos, 1.0f);
	v_Color = aColor;
	v_TexCoords = aTexCoords;
}