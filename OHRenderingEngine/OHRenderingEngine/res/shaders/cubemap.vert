#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 v_TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 pos = projection * view  * vec4(aPos, 1.0f);
	gl_Position = pos.xyww;
	v_TexCoords = vec3(-aPos.x, aPos.y, aPos.z);
}