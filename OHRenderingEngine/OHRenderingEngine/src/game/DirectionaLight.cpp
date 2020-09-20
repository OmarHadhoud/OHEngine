#include <glm/gtc/matrix_transform.hpp>

#include "DirectionaLight.h"


DirectionaLight::DirectionaLight()
	: m_Direction(glm::vec3(0.0f))
{
}

DirectionaLight::DirectionaLight(glm::vec3 direction, glm::vec3 color, float ambient, float diffuse, float specular, bool enabled):
	Light(color, ambient, diffuse, specular, enabled),m_Direction(direction)
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


void DirectionaLight::UpdateTransformationMatrix()
{
	float FrustumSize = GetFrustumSize();
	float NearPlane = GetNearPlane();
	float FarPlane = GetFarPlane();
	glm::mat4 ViewMatrix = glm::lookAt(-m_Direction,glm::vec3(0.0f),glm::vec3(0.0f,1.0f,0.0f));
	glm::mat4 ProjectionMatrix = glm::ortho(-FrustumSize, FrustumSize, -FrustumSize, FrustumSize, NearPlane, FarPlane);
	glm::mat4 TransformationMatrix = ProjectionMatrix * ViewMatrix;
	SetTransformationMatrix(std::vector<glm::mat4> (1,TransformationMatrix));
}
