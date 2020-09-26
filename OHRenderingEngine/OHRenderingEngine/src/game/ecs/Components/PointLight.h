#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/glm.hpp>

struct PointLight
{
	glm::vec3 m_Color;
	glm::vec3 m_Position;
	float m_Ambient;
	float m_Diffuse;
	float m_Specular;
	float m_kConstant;
	float m_kLinear;
	float m_kQuadratic;
	bool m_Enabled;
};

#endif // !POINT_LIGHT_H

