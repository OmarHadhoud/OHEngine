#version 400 core

in GS_OUT
{
	vec2 v_TexCoords;
	vec4 fragPosWorld;
} fs_in;

uniform float FarPlane;
uniform vec3 LightPosition;

//Material structs
struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	float shineness;
	bool blinn;
};


//Materials
uniform Material material;


void main()
{
	float alpha = texture(material.texture_diffuse1, fs_in.v_TexCoords).a;
	if(alpha <0.1)
		discard;
	float dist = length(LightPosition - fs_in.fragPosWorld.xyz);
	dist = dist / FarPlane;
	gl_FragDepth = dist;
}