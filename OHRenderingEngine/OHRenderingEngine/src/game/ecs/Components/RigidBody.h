#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <unordered_map>
#include <glm/glm.hpp>

struct RigidBody
{
	glm::vec3 m_Velocity;
	glm::vec3 m_Acceleration;
	float m_Weight;
	bool m_Enabled;
	static unsigned int m_Count;
	static std::unordered_map<unsigned int, unsigned int> m_Map;
};

#endif // !RIGID_BODY_H

