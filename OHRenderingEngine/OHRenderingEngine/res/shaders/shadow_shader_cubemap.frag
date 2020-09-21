#version 400 core

in GS_OUT
{
	vec4 fragPosWorld;
} fs_in;

uniform float FarPlane;
uniform vec3 LightPosition;

void main()
{
	float dist = length(LightPosition - fs_in.fragPosWorld.xyz);
	dist = dist / FarPlane;
	gl_FragDepth = dist;
}