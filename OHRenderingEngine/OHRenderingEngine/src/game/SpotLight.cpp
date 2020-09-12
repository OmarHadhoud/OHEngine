#include "SpotLight.h"



SpotLight::SpotLight()
{
}

SpotLight::SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 color, float ambient, float diffuse, float specular, float kC, float kL, float kQ, float inner_cutoff, float outer_cutoff, bool enabled)
	: PointLight(pos, color, ambient, diffuse, specular, kC, kL, kQ, enabled), m_Direction(dir), m_InnerCutoff(inner_cutoff), m_OuterCutoff(outer_cutoff)
{
}


SpotLight::~SpotLight()
{
}

glm::vec3 SpotLight::GetDirection() const
{
	return m_Direction;
}

float SpotLight::GetInnerCutoff() const
{
	return m_InnerCutoff;
}

float SpotLight::GetOuterCutoff() const
{
	return m_OuterCutoff;
}

void SpotLight::SetDirection(glm::vec3 dir)
{
	m_Direction = dir;
}

void SpotLight::SetInnerCutoff(float val)
{
	assert(val >= 0);
	m_InnerCutoff = val;
}

void SpotLight::SetOuterCutoff(float val)
{
	assert(val >= 0);
	m_OuterCutoff = val;
}
