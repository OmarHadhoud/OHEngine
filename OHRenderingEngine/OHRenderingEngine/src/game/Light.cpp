#include "Light.h"



Light::Light():
	m_Color(glm::vec3(0.0f)),
	m_Ambient((0.0f)),
	m_Diffuse((0.0f)),
	m_Specular((0.0f))
{
}

Light::Light(glm::vec3 color, float ambient, float diffuse, float specular)
	: m_Color(color), m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular)
{
}


Light::~Light()
{
}

glm::vec3 Light::GetColor() const
{
	return m_Color;
}

float Light::GetAmbient() const
{
	return m_Ambient;
}

float Light::GetDiffuse() const
{
	return m_Diffuse;
}

float Light::GetSpecular() const
{
	return m_Specular;
}

void Light::SetColor(glm::vec3 color)
{
	m_Color = color;
}

void Light::SetAmbient(float ambient)
{
	m_Ambient = ambient;
}

void Light::SetDiffuse(float diffuse)
{
	m_Diffuse = diffuse;
}

void Light::SetSpecular(float specular)
{
	m_Specular = specular;
}
