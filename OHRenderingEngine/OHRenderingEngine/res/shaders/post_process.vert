#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 v_TexCoords;

void main()
{
    gl_Position = vec4(aPos,1.0f, 1.0f);
	v_TexCoords = aTexCoords;
}