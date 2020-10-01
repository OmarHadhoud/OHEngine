#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <glm/glm.hpp>
#include "game/ecs/Components/Component.h"

struct RigidBody : Component
{
	glm::vec3 m_Velocity;
	glm::vec3 m_Acceleration;
	float m_Weight;
	static unsigned int m_Count;
	static int m_Indices[MAX_ENTITY_COUNT];
};

#endif // !RIGID_BODY_H

