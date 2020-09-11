#include "DirectionaLight.h"



DirectionaLight::DirectionaLight()
	: m_Direction(glm::vec3(0.0f))
{
}

DirectionaLight::DirectionaLight(glm::vec3 direction, glm::vec3 color, float ambient, float diffuse, float specular):
	Light(color, ambient, diffuse, specular),m_Direction(direction)	
{
}


DirectionaLight::~DirectionaLight()
{
}

glm::vec3 DirectionaLight::GetDirection() const
{
	return m_Direction;
}

void DirectionaLight::SetDirection(glm::vec3 dir)
{
	m_Direction = dir;
}
