#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include <glm/glm.hpp>

struct BoxCollider
{
	glm::vec3 m_Bounds[2];
	bool m_Enabled;
};

#endif // !BOX_COLLIDER_H

