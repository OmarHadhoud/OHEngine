#version 400 core
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
	vec3 fragPosWorld;
	mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 view;

void main()
{
    gl_Position = view * model * vec4(aPos, 1.0f);
	vs_out.v_TexCoords = vec2(aTexCoords.x, aTexCoords.y);
	vs_out.v_NormalDir = mat3(transpose(inverse(view * model))) * aNormal;
	vs_out.fragPos = vec3(view * model * vec4(aPos, 1.0f));
	vs_out.fragPosWorld = vec3(model * vec4(aPos, 1.0f));

	vec3 normal = normalize(vec3(view*model*vec4(aNormal,0.0f)));
	vec3 tangent = normalize(vec3(view*model*vec4(aTangent,0.0f)));
	vec3 bitangent = normalize(vec3(view*model*vec4(aBitangent,0.0f)));
	//Gram shmidt orthogonalization
	normal = normalize(normal - tangent * dot(tangent, normal));
	bitangent = normalize(cross(normal, tangent));
	vs_out.TBN = mat3(tangent, bitangent, normal);
}