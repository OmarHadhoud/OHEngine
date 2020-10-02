#version 400 core

in VS_OUT
{
	vec2 v_TexCoords;
} fs_in;


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
}