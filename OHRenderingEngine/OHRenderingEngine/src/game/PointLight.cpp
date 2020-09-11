#include "PointLight.h"



PointLight::PointLight()
{
}

PointLight::PointLight(glm::vec3 pos, glm::vec3 color, float ambient, float diffuse, float specular, float kC, float kL, float kQ)
	: Light(color, ambient, diffuse, specular),m_Position(pos), m_kC(kC), m_kL(kL), m_kQ(kQ)
{
}


PointLight::~PointLight()
{
}

glm::vec3 PointLight::GetPosition() const
{
	return m_Position;
}

float PointLight::GetConstant() const
{
	return m_kC;
}

float PointLight::GetLinearConstant() const
{
	return m_kL;
}

float PointLight::GetQuadraticConstant() const
{
	return m_kQ;
}

void PointLight::SetPosition(glm::vec3 pos)
{
	m_Position = pos;
}

void PointLight::SetConstant(float val)
{
	assert(val >= 0);
	m_kC = val;
}

void PointLight::SetLinearConstant(float val)
{
	assert(val >= 0);
	m_kL = val;
}

void PointLight::SetQuadraticConstant(float val)
{
	assert(val >= 0);
	m_kQ = val;
}
