#version 400 core
layout (location = 0) in vec3 aPos;

uniform mat4 TransformationMatrix;
uniform mat4 model;

void main()
{
	vec3 worldPos = vec3(model * vec4(aPos, 1.0f));
    gl_Position = TransformationMatrix * vec4(worldPos,1.0f);
}