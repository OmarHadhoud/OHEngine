#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shineness;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float outerCutOff;
	float innerCutOff;
};



uniform Material material;

#define NUM_POINT_LIGHTS 4
uniform DirLight dirLight;
uniform PointLight pointLight[NUM_POINT_LIGHTS];
uniform SpotLight spotLight;

//Functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

void main()
{
	//Shared data between different Functions
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalcDirLight(dirLight, normal, viewDir);
	for(int i = 0; i < NUM_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLight[i], normal, viewDir);
	result += CalcSpotLight(spotLight, normal, viewDir);

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//Vectors needed
	vec3 lightDir = normalize(-light.direction);
	
	//ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;


	//diffuse
	float diff = max(dot(normal, lightDir),0.0);
	vec3 diffuse = (diff * texture(material.diffuse, TexCoords).rgb) * light.diffuse;

	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir),0.0), material.shineness);
	vec3 specular = (spec * texture(material.specular, TexCoords).rgb)* light.specular;

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	//Vectors needed
	vec3 lightDir = normalize(light.position - FragPos);

	
	//ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	
	//diffuse
	float diff = max(dot(normal, lightDir),0.0);
	vec3 diffuse = (diff * texture(material.diffuse, TexCoords).rgb) * light.diffuse;

	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir),0.0), material.shineness);
	vec3 specular = (spec * texture(material.specular, TexCoords).rgb)* light.specular;
	
	float distance = length(light.position - FragPos);
	float attenuation = (1.0f) / (light.constant + light.linear*distance + light.quadratic*(distance*distance) );

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
//Vectors needed
	vec3 lightDir = normalize(light.position - FragPos);

	
	//ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	
	//diffuse
	float diff = max(dot(normal, lightDir),0.0);
	vec3 diffuse = (diff * texture(material.diffuse, TexCoords).rgb) * light.diffuse;

	//specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir),0.0), material.shineness);
	vec3 specular = (spec * texture(material.specular, TexCoords).rgb)* light.specular;
	
	float distance = length(light.position - FragPos);
	float attenuation = (1.0f) / (light.constant + light.linear*distance + light.quadratic*(distance*distance) );

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta-light.outerCutOff)/epsilon, 0.0, 1.0);

	ambient *= attenuation;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}