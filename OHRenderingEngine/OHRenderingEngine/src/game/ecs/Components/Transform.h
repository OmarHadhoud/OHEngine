#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

struct Tranform
{
	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_RotationAxis;
	float m_RotationAngle;
	glm::vec3 m_Forward;
	glm::mat4 m_ModelMatrix;
	bool m_Enabled;
};

#endif // !TRANSFORM_H

