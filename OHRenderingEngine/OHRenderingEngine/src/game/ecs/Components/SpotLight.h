#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <glm/glm.hpp>

struct SpotLight
{
	glm::vec3 m_Color;
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	float m_Ambient;
	float m_Diffuse;
	float m_Specular;
	float m_kConstant;
	float m_kLinear;
	float m_kQuadratic;
	float m_InnerCutoff;
	float m_InnerCutoffAngle;
	float m_OuterCutoff;
	float m_OuterCutoffAngle;
	bool m_Enabled;
};

#endif // !SPOT_LIGHT_H

