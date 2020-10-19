#ifndef HEALTH_H
#define HEALTH_H

#include <unordered_map>
#include <glm/glm.hpp>
#include "game/ecs/Components/Component.h"

struct Health : Component
{
	float m_Value;
	static unsigned int m_Count;
	static int m_Indices[MAX_ENTITY_COUNT];
};

#endif // !HEALTH_H

