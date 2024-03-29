#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/glm.hpp>
#include "game/ecs/Components/Component.h"

struct DirectionalLight : Component
{
	glm::vec3 m_Color;
	glm::vec3 m_Direction;
	float m_Ambient;
	float m_Diffuse;
	float m_Specular;
	static unsigned int m_Count;
	static int m_Indices[MAX_ENTITY_COUNT];
};

#endif // !DIRECTIONAL_LIGHT_H

