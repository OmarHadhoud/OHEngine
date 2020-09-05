#version 330 core

out vec4 FragColor;
in vec3 v_Color;
in vec4 v_NormalDir;
in vec2 v_TexCoords;
in vec3 fragPos;

struct DirectionaLighting
{
	vec3 direction;

	vec3 color;

	float ambient;
	float diffuse;
	float specular;
};

struct PointLight
{
	vec3 position;

	vec3 color;

	float ambient;
	float diffuse;
	float specular;

	float kC;
	float kL;
	float kQ;
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
};


uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform mat4 normal;
uniform vec3 lightColor;

//The lightings
#define MAX_DIRECTIONAL_LIGHTS  5
#define MAX_POINT_LIGHTS  20
#define MAX_SPOTLIGHTS 20
uniform int n_DirectionalLights;	//Actual number of directional lights passed
uniform int n_PointLights;			//Actual number of point lights passed
uniform int n_SpotLights;			//Actual number of spotlights passed
uniform DirectionaLighting directionalLighting[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOTLIGHTS];

//Light computation functions
vec3 ComputeLight();
vec3 ComputeDirectionalLight();
vec3 ComputePointLights();
vec3 ComputeSpotLights();

void main()
{	
	FragColor = vec4(ComputeLight(), 1.0f) * texture(texture0, v_TexCoords);
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
		vec3 lightDir = - normalize(directionalLighting[i].direction);

		//Ambient
		ambient = vec3(directionalLighting[i].ambient);

		//Diffuse
		vec3 normalDir = vec3(normalize(normal * v_NormalDir));
		float diffuse_dot = max(dot(normalDir, lightDir), 0.0f);

		diffuse = vec3(diffuse_dot*directionalLighting[i].diffuse);

		//Specular
		vec3 reflectDir = normalize(reflect(-lightDir, normalDir));
		vec3 viewDir = normalize(viewPos - fragPos);
		float specular_dot = max(dot(reflectDir, viewDir), 0.0f);

		specular = vec3(pow(specular_dot,32)* directionalLighting[i].specular);
		ret += directionalLighting[i].color * (ambient+diffuse+specular);
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
		vec3 lightDir = normalize(pointLights[i].position - fragPos);

		//Ambient
		ambient = vec3(pointLights[i].ambient);

		//Diffuse
		vec3 normalDir = vec3(normalize(normal * v_NormalDir));
		float diffuse_dot = max(dot(normalDir, lightDir), 0.0f);

		diffuse = vec3(diffuse_dot*pointLights[i].diffuse);

		//Specular
		vec3 reflectDir = normalize(reflect(-lightDir, normalDir));
		vec3 viewDir = normalize(viewPos - fragPos);
		float specular_dot = max(dot(reflectDir, viewDir), 0.0f);

		specular = vec3(pow(specular_dot,32)* pointLights[i].specular);
		//Attenuate
		float dist = distance(fragPos, pointLights[i].position);
		float f_attn = 1.0f / (pointLights[i].kC + pointLights[i].kL * dist + pointLights[i].kQ * dist * dist); 

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
		vec3 lightDir = normalize(spotLights[i].position - fragPos);

		//Ambient
		ambient = vec3(spotLights[i].ambient);

		//Diffuse
		vec3 normalDir = vec3(normalize(normal * v_NormalDir));
		float diffuse_dot = max(dot(normalDir, lightDir), 0.0f);

		diffuse = vec3(diffuse_dot*spotLights[i].diffuse);

		//Specular
		vec3 reflectDir = normalize(reflect(-lightDir, normalDir));
		vec3 viewDir = normalize(viewPos - fragPos);
		float specular_dot = max(dot(reflectDir, viewDir), 0.0f);

		specular = vec3(pow(specular_dot,32)* spotLights[i].specular);
		//Attenuate
		float dist = distance(fragPos, spotLights[i].position);
		float f_attn = 1.0f / (spotLights[i].kC + spotLights[i].kL * dist + spotLights[i].kQ * dist * dist); 
		//Cutoff parts outside of the cutoff angles
		float theta = dot(lightDir, normalize(-spotLights[i].direction));
		float epsilon = spotLights[i].inner_cutoff - spotLights[i].outer_cutoff;
		float intensity = clamp((theta - spotLights[i].outer_cutoff) / epsilon , 0.0f, 1.0f);
		diffuse *= intensity;
		specular *= intensity;
		
		ret += f_attn *spotLights[i].color * (ambient+diffuse+specular);
	}
	
	return ret;
}