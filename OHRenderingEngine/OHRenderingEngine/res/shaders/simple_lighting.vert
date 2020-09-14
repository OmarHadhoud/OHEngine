#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT
{
	vec3 v_NormalDir;
	vec2 v_TexCoords;
	vec3 fragPos;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection*view * model * vec4(aPos, 1.0f);
	vs_out.v_TexCoords = vec2(aTexCoords.x, aTexCoords.y);
	vs_out.v_NormalDir = mat3(transpose(inverse(model))) * aNormal;
	vs_out.fragPos = vec3(model * vec4(aPos, 1.0f));
}