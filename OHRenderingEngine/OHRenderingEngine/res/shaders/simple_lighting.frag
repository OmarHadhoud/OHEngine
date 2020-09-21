#version 400 core

out vec4 FragColor;

in GS_OUT
{
	vec3 v_NormalDir;
	vec2 v_TexCoords;
	vec3 fragPos;
	vec3 fragPosWorld;
} fs_in;

//Lighting structs
struct DirectionaLighting
{
	vec3 direction;

	vec3 color;

	float ambient;
	float diffuse;
	float specular;

	mat4 TransformationMatrix;
	sampler2D DepthMap;
};

struct PointLight
{
	vec3 position;
	vec3 world_position;

	vec3 color;

	float ambient;
	float diffuse;
	float specular;

	float kC;
	float kL;
	float kQ;

	samplerCube DepthMap;
	float far_plane;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	vec3 color;

	float kC;
	float kL;
	float kQ;

	float ambient;
	float diffuse;
	float specular;

	float outer_cutoff;
	float inner_cutoff;

	mat4 TransformationMatrix;
	sampler2D DepthMap;
};

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
uniform samplerCube ssss;

//The lightings
#define MAX_DIRECTIONAL_LIGHTS  5
#define MAX_POINT_LIGHTS  20
#define MAX_SPOTLIGHTS 20
uniform int n_DirectionalLights;	//Actual number of directional lights passed
uniform int n_PointLights;			//Actual number of point lights passed
uniform int n_SpotLights;			//Actual number of spotlights passed
uniform DirectionaLighting directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOTLIGHTS];


//Light computation functions
vec3 ComputeLight();
vec3 ComputeDirectionalLight();
vec3 ComputePointLights();
vec3 ComputeSpotLights();
float CalcShadow(mat4 TransformationMatrix ,sampler2D DepthMap, vec3 normalDir, vec3 lightDir);
float CalcShadow(samplerCube DepthMap, float far_plane, vec3 normalDir, vec3 lightDir, float viewDistance );


void main()
{	
	FragColor = vec4(ComputeLight(), texture(material.texture_diffuse1, fs_in.v_TexCoords).a);
	if(FragColor.a < 0.1)
		discard;
}


vec3 ComputeLight()
{
	vec3 color = ComputeDirectionalLight();
	color += ComputePointLights();
	color += ComputeSpotLights();
	return color;
}

vec3 ComputeDirectionalLight()
{
	vec3 ret = vec3(0.0f);
	vec3 ambient = vec3(0.0f);
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	//Loop on all directional lights in the scene
	for(int i = 0; i < n_DirectionalLights; i++)
	{
		//Inverse the directional lighting direction
		vec3 lightDir = - normalize(directionalLights[i].direction);

		//Ambient
		ambient = vec3(directionalLights[i].ambient) * vec3(texture(material.texture_diffuse1, fs_in.v_TexCoords));

		//Diffuse
		vec3 normalDir = vec3(normalize(fs_in.v_NormalDir));
		float diffuse_dot = max(dot(normalDir, lightDir), 0.0f);

		diffuse = vec3(diffuse_dot*directionalLights[i].diffuse) *vec3(texture(material.texture_diffuse1, fs_in.v_TexCoords));

		//Specular
		vec3 reflectDir = normalize(reflect(-lightDir, normalDir));
		vec3 viewDir = normalize( - fs_in.fragPos);
		vec3 halfWayDir = normalize(viewDir+lightDir);
		float specular_dot;
		if(!material.blinn)
			specular_dot = max(dot(reflectDir, viewDir), 0.0f);
		else
			specular_dot = max(dot(halfWayDir, normalDir), 0.0f);
		specular = vec3(pow(specular_dot,material.shineness)* directionalLights[i].specular) * vec3(texture(material.texture_specular1, fs_in.v_TexCoords));
		//Shadow calculation
		float shadow = CalcShadow(directionalLights[i].TransformationMatrix, directionalLights[i].DepthMap, normalDir, lightDir);
		specular*=(1-shadow);
		diffuse*=(1-shadow);
		ret += directionalLights[i].color * (ambient+diffuse+specular);
	}
	
	return ret;
}

vec3 ComputePointLights()
{
	vec3 ret = vec3(0.0f);
	vec3 ambient = vec3(0.0f);
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	//Loop on all point lights in the scene
	for(int i = 0; i < n_PointLights; i++)
	{
		//Get the direction of the light
		vec3 lightDir = normalize(pointLights[i].position - fs_in.fragPos);

		//Ambient
		ambient = vec3(pointLights[i].ambient) * vec3(texture(material.texture_diffuse1, fs_in.v_TexCoords));

		//Diffuse
		vec3 normalDir = vec3(normalize(fs_in.v_NormalDir));
		float diffuse_dot = max(dot(normalDir, lightDir), 0.0f);

		diffuse = vec3(diffuse_dot*pointLights[i].diffuse) * vec3(texture(material.texture_diffuse1, fs_in.v_TexCoords));

		//Specular
		vec3 reflectDir = normalize(reflect(-lightDir, normalDir));
		vec3 viewDir = normalize( - fs_in.fragPos);
		vec3 halfWayDir = normalize(viewDir+lightDir);
		float specular_dot;
		if(!material.blinn)
			specular_dot = max(dot(reflectDir, viewDir), 0.0f);
		else
			specular_dot = max(dot(halfWayDir, normalDir), 0.0f);

		specular = vec3(pow(specular_dot,material.shineness)* pointLights[i].specular) * vec3(texture(material.texture_specular1, fs_in.v_TexCoords));
		//Attenuate
		float dist = distance(fs_in.fragPos, pointLights[i].position);
		float f_attn = 1.0f / (pointLights[i].kC + pointLights[i].kL * dist + pointLights[i].kQ * dist * dist); 

		//Shadow calculation
		vec3 lightDirNonNorm = fs_in.fragPosWorld- pointLights[i].world_position;
		float shadow = CalcShadow(pointLights[i].DepthMap, pointLights[i].far_plane, normalDir, lightDirNonNorm, length(fs_in.fragPos));
		specular*=(1-shadow);
		diffuse*=(1-shadow);

		ret += f_attn * pointLights[i].color * (ambient+diffuse+specular);
	}
	
	return ret;
}

vec3 ComputeSpotLights()
{
	vec3 ret = vec3(0.0f);
	vec3 ambient = vec3(0.0f);
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	//Loop on all spotlights in the scene
	for(int i = 0; i < n_SpotLights; i++)
	{
		//Get the direction of the light
		vec3 lightDir = normalize(spotLights[i].position - fs_in.fragPos);

		//Ambient
		ambient = vec3(spotLights[i].ambient) * vec3(texture(material.texture_diffuse1, fs_in.v_TexCoords));

		//Diffuse
		vec3 normalDir = vec3(normalize(fs_in.v_NormalDir));
		float diffuse_dot = max(dot(normalDir, lightDir), 0.0f);

		diffuse = vec3(diffuse_dot*spotLights[i].diffuse) * vec3(texture(material.texture_diffuse1, fs_in.v_TexCoords));

		//Specular
		vec3 reflectDir = normalize(reflect(-lightDir, normalDir));
		vec3 viewDir = normalize( - fs_in.fragPos);
		vec3 halfWayDir = normalize(viewDir+lightDir);
		float specular_dot;
		if(!material.blinn)
			specular_dot = max(dot(reflectDir, viewDir), 0.0f);
		else
			specular_dot = max(dot(halfWayDir, normalDir), 0.0f);

		specular = vec3(pow(specular_dot,material.shineness)* spotLights[i].specular) * vec3(texture(material.texture_specular1, fs_in.v_TexCoords));
		//Attenuate
		float dist = distance(fs_in.fragPos, spotLights[i].position);
		float f_attn = 1.0f / (spotLights[i].kC + spotLights[i].kL * dist + spotLights[i].kQ * dist * dist); 
		//Cutoff parts outside of the cutoff angles
		float theta = dot(lightDir, normalize(-spotLights[i].direction));
		float epsilon = spotLights[i].inner_cutoff - spotLights[i].outer_cutoff;
		float intensity = clamp((theta - spotLights[i].outer_cutoff) / epsilon , 0.0f, 1.0f);
		diffuse *= intensity;
		specular *= intensity;
		
		//Shadow calculation
		float shadow = CalcShadow(spotLights[i].TransformationMatrix, spotLights[i].DepthMap, normalDir, lightDir);
		specular*=(1-shadow);
		diffuse*=(1-shadow);

		ret += f_attn *spotLights[i].color * (ambient+diffuse+specular);
	}
	
	return ret;
}

float CalcShadow(mat4 TransformationMatrix ,sampler2D DepthMap, vec3 normalDir, vec3 lightDir)
{
	vec2 texelSize = 1.0/textureSize(DepthMap,0);
	vec4 FragLightPos = TransformationMatrix * vec4(fs_in.fragPosWorld, 1.0f);
	vec3 lightCoords = FragLightPos.xyz / FragLightPos.w;
	lightCoords = 0.5f * lightCoords + 0.5f;
	float shadow = 0.0f;
	float bias = max(0.005f, 0.05*(1-dot(normalDir, lightDir)));
	bias = 0.0f; //Using culling, no need for bias here.
	float currentPoint = lightCoords.z;
	for(int x = -1; x < 2; x++)
	{
		for(int y = -1; y < 2; y++)
		{
			vec2 offset = texelSize * vec2(x,y);
			float closestPoint  = texture(DepthMap, lightCoords.xy+offset).r;
			shadow += currentPoint - bias > closestPoint ? 1.0f : 0.0f;
		}
	}
	shadow /= 9.0f;
	if(currentPoint > 1)
		shadow = 0;	
	return shadow;
}

vec3 sampleDirectionsPtLight[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float CalcShadow(samplerCube DepthMap, float far_plane, vec3 normalDir, vec3 lightDir, float viewDistance)
{
	//DepthMap = ssss;//for now

	vec3 lightToFragment = lightDir;
	float currentPoint = length(lightToFragment);
	float shadow = 0.0f;
	float bias = max(0.005f, 0.05*(1-dot(normalDir, lightToFragment)));
	bias = 0.0f; //Using culling, no need for bias here.
	float diskRadius = (1 + (viewDistance/far_plane) ) / 25.0f;
	for(int i = 0; i < 20; i++)
	{
		float closestPoint = texture(DepthMap, lightToFragment + diskRadius * sampleDirectionsPtLight[i]).r;
		closestPoint *= far_plane;
		shadow += currentPoint - bias > closestPoint ? 1.0f : 0.0f;
	}
	shadow /= 20.0f;
	return shadow;
}