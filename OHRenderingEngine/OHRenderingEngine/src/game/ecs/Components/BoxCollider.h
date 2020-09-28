#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include <unordered_map>
#include <glm/glm.hpp>

struct BoxCollider
{
	glm::vec3 m_Bounds[2];
	bool m_Enabled;
	static unsigned int m_Count;
	static std::unordered_map<unsigned int, unsigned int> m_Map;
};

#endif // !BOX_COLLIDER_H

