#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include <unordered_map>
#include <glm/glm.hpp>

#include "game/ecs/Components/Component.h"

struct BoxCollider : Component
{
	glm::vec3 m_Bounds[2];
	static unsigned int m_Count;
 static std::unordered_map<int, int> m_Map;
};

#endif // !BOX_COLLIDER_H

