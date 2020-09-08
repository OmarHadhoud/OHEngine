#version 330 core

out vec4 FragColor;
in vec2 v_TexCoords;

uniform sampler2D quadTex;

const float offset = 1.0f / 300.0f;

vec2 dir[9] = vec2[]
(
	vec2(-offset,	 offset),	//Top left
	vec2(-offset,	 0),		//Left
	vec2(-offset,	-offset),	//Bottom left
	vec2(0,			 offset),	//Top
	vec2(0,			 0),		//This pixel
	vec2(0,			-offset),	//Bottom
	vec2(offset,	 offset),	//Top right
	vec2(offset,	 0),		//Reft
	vec2(offset,	-offset)	//Bottom right
);

float kernel[9] = float[]
(
	 0,0,0,
	 0,1,0,
	 0,0,0
);

float kernelConstant = 1;

vec3 pixels[9];

void main()
{
	for(int i = 0; i < 9; i++)
		pixels[i] = vec3(texture(quadTex, v_TexCoords.xy+dir[i]));
	FragColor = vec4(0.0f,0.0f,0.0f,1.0f);
	for(int i = 0; i < 9; i++)
		FragColor += kernel[i]*vec4(pixels[i], 0.0f);
	FragColor *= vec4(vec3(kernelConstant),1.0f);
}