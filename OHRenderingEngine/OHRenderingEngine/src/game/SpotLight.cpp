#include <glm/gtc/matrix_transform.hpp>

#include "SpotLight.h"



SpotLight::SpotLight()
{
}

SpotLight::SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 color, float ambient, float diffuse, float specular, float kC, float kL, float kQ, float inner_cutoff, float outer_cutoff, bool enabled)
	: PointLight(pos, color, ambient, diffuse, specular, kC, kL, kQ, enabled), m_Direction(dir), m_InnerCutoff(cos(inner_cutoff)), m_OuterCutoff(cos(outer_cutoff)), m_InnerCutoffAngle(inner_cutoff), m_OuterCutoffAngle(outer_cutoff)
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
	m_InnerCutoff = cos(val);
	m_InnerCutoffAngle = val;
}

void SpotLight::SetOuterCutoff(float val)
{
	assert(val >= 0);
	m_OuterCutoff = cos(val);
	m_OuterCutoffAngle = val;
}

void SpotLight::UpdateTransformationMatrix()
{
	float FrustumSize = GetFrustumSize();
	float NearPlane = GetNearPlane();
	float FarPlane = GetFarPlane();
	glm::mat4 ViewMatrix = glm::lookAt(GetPosition(), GetPosition() + GetDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(90.0f), (float)1.0f, NearPlane, FarPlane);
	glm::mat4 TransformationMatrix = ProjectionMatrix * ViewMatrix;
	SetTransformationMatrix(TransformationMatrix);
}
