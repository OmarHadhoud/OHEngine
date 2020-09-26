#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <glm/glm.hpp>

struct RigidBody
{
	glm::vec3 m_Velocity;
	glm::vec3 m_Acceleration;
	float m_Weight;
	bool m_Enabled;
};

#endif // !RIGID_BODY_H

