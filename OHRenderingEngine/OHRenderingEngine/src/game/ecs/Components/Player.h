#ifndef PLAYER_H
#define PLAYER_H

#include <unordered_map>
#include <glm/glm.hpp>
#include "game/ecs/Components/Component.h"

struct Player : Component
{
	static unsigned int m_Count;
	static int m_Indices[MAX_ENTITY_COUNT];
};

#endif // !PLAYER_H

