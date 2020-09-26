#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/glm.hpp>

struct DirectionalLight
{
	glm::vec3 m_Color;
	glm::vec3 m_Direction;
	float m_Ambient;
	float m_Diffuse;
	float m_Specular;
	bool m_Enabled;
};

#endif // !DIRECTIONAL_LIGHT_H

