#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aNormalCoords;

out vec3 v_Color;
out vec4 v_NormalDir;
out vec2 v_TexCoords;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
	v_Color = aColor;
	v_TexCoords = vec2(aTexCoords.x, aTexCoords.y);
	v_NormalDir = aNormalCoords;
	fragPos = vec3(model * vec4(aPos, 1.0f));
}