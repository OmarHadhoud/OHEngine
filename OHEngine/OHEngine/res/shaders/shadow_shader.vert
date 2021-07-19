#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in mat4 aInstanceMatrix;

uniform mat4 TransformationMatrix;
uniform mat4 model;


out VS_OUT
{
	vec2 v_TexCoords;
} vs_out;


void main()
{
    gl_Position = TransformationMatrix * model * vec4(aPos, 1.0f);
	vs_out.v_TexCoords = vec2(aTexCoords.x, aTexCoords.y);
}