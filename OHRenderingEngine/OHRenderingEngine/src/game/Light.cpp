#include "Light.h"



Light::Light():
	m_Color(glm::vec3(0.0f)),
	m_Ambient((0.0f)),
	m_Diffuse((0.0f)),
	m_Specular((0.0f)),
	m_TransformationMatrix(glm::mat4(1.0f)),
	m_FarPlane(10.0f),
	m_NearPlane(1.0f),
	m_FrustumSize(10.0f)
{
}

Light::Light(glm::vec3 color, float ambient, float diffuse, float specular, bool enabled)
	: m_Color(color), m_Ambient(ambient), m_Diffuse(diffuse), m_Specular(specular), m_Enabled(enabled), m_TransformationMatrix(glm::mat4(1.0f)),
	m_FarPlane(10.0f),
	m_NearPlane(1.0f),
	m_FrustumSize(10.0f)
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

float Light::GetFrustumSize() const
{
	return m_FrustumSize;
}

float Light::GetNearPlane() const
{
	return m_NearPlane;
}

float Light::GetFarPlane() const
{
	return m_FarPlane;
}

glm::mat4 Light::GetTransformationMatrix() const
{
	return m_TransformationMatrix;
}

int Light::GetDepthMap() const
{
	return m_DepthMap;
}

bool Light::IsEnabled() const
{
	return m_Enabled;
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

void Light::SetDepthMap(int map)
{
	m_DepthMap = map;
}

void Light::SetTransformationMatrix(glm::mat4 transformation_matrix)
{
	m_TransformationMatrix = transformation_matrix;
}

void Light::Enable()
{
	m_Enabled = true;
}

void Light::Disable()
{
	m_Enabled = false;
}



void Light::SetFrustumSize(float size)
{
	m_FrustumSize = size;
}

void Light::SetNearPlane(float value)
{
	m_NearPlane = value;
}

void Light::SetFarPlane(float value)
{
	m_FarPlane = value;
}