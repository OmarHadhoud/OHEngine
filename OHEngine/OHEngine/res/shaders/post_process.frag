#version 400 core

out vec4 FragColor;
in vec2 v_TexCoords;

uniform sampler2D quadTex;
uniform sampler2D brightTex;
uniform sampler2D crosshairTex;
uniform sampler2D gunTex;
uniform sampler2D gunfireTex;
uniform bool shootingFire;
uniform bool moving;
uniform float recoilPower;
uniform float blurStrength;
uniform float gammaCorrection;
uniform float exposure;
uniform vec2 resolution;

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
	 1,2,1,
	 2,4,2,
	 1,2,1
);

float kernelConstant = 1.0f/16.0f;

vec3 pixels[9];
vec3 pixels_bright[9];


void main()
{
	for(int i = 0; i < 9; i++)
	{
		pixels[i] = vec3(texture(quadTex, v_TexCoords.xy+dir[i]));
		pixels_bright[i] = vec3(texture(brightTex, v_TexCoords.xy+dir[i]));

	}
	FragColor = vec4(0.0f,0.0f,0.0f,1.0f);
	vec4 BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for(int i = 0; i < 9; i++)
	{
		vec4 col = kernel[i] * vec4(pixels[i], 0.0f);
		FragColor += col;
		col = kernel[i] * vec4(pixels_bright[i],0.0f);
		BrightColor += col;
	}
	FragColor *= vec4(vec3(kernelConstant),1.0f);
	FragColor = (1-blurStrength)*vec4(pixels[4],1.0f) + blurStrength*FragColor;
	BrightColor *= vec4(vec3(kernelConstant), 1.0f);
	BrightColor = (1-blurStrength)*vec4(pixels_bright[4],1.0f) + blurStrength*BrightColor;
	FragColor += vec4(vec3(BrightColor),0.0f);
	//Do tone mapping 
	FragColor = vec4(vec3(1.0f) - exp(-FragColor.xyz*exposure),1.0f);
	//Add crosshair on top [Hard coded values, should be modularized or added to HUD shader later on]
	float invRatio = resolution.y / resolution.x;
	vec3 crosshair = vec3(texture(crosshairTex, vec2(19,19*invRatio)*vec2(-0.474,-0.474)+(v_TexCoords.xy*vec2(19,19*invRatio))));
	FragColor += vec4(crosshair, 0.0f);
	//Add gunfire
	vec4 gun = vec4(texture(gunfireTex, (v_TexCoords.xy)*vec2(1,1*invRatio))); 
		if(gun.a > 0.1f && shootingFire==true)
		FragColor += gun ;
	//Make gamma correction
	FragColor = vec4(pow(vec3(FragColor),vec3(1.0f/gammaCorrection)),1.0f);
	//Shdaow map debugging
	//FragColor = vec4(vec3(texture(quadTex,v_TexCoords).r),1.0f);
}