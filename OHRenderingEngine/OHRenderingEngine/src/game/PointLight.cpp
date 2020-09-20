#include <glm/gtc/matrix_transform.hpp>

#include "PointLight.h"


glm::vec3 directions[6] = {
	glm::vec3(1,0,0),
	glm::vec3(-1,0,0),
	glm::vec3(0,1,0),
	glm::vec3(0,-1,0),
	glm::vec3(0,0,1),
	glm::vec3(0,0,-1)
};

glm::vec3 Up[6] = {
	glm::vec3(0,-1,0),
	glm::vec3(0,-1,0),
	glm::vec3(0,0,1),
	glm::vec3(0,0,-1),
	glm::vec3(0,-1,0),
	glm::vec3(0,-1,0)
};


PointLight::PointLight()
{
}

PointLight::PointLight(glm::vec3 pos, glm::vec3 color, float ambient, float diffuse, float specular, float kC, float kL, float kQ, bool enabled)
	: Light(color, ambient, diffuse, specular, enabled),m_Position(pos), m_kC(kC), m_kL(kL), m_kQ(kQ)
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

void PointLight::UpdateTransformationMatrix()
{
	float NearPlane = GetNearPlane();
	float FarPlane = GetFarPlane();
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(90.0f), (float)1.0f, NearPlane, FarPlane);
	std::vector<glm::mat4> matrices;
	for (int i = 0; i < 6; i++)
	{
		glm::mat4 ViewMatrix = glm::lookAt(GetPosition(), GetPosition()+directions[i], Up[i]);
		glm::mat4 TransformationMatrix = ProjectionMatrix * ViewMatrix;
		matrices.push_back(TransformationMatrix);
	}
	SetTransformationMatrix(matrices);
}
