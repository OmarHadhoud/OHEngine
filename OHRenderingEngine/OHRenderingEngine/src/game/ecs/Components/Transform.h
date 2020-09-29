#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

#include "game/ecs/Components/Component.h"

struct Transform : Component
{
	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_RotationAxis;
	float m_RotationAngle;
	glm::vec3 m_Forward;
	glm::mat4 m_ModelMatrix;
	static unsigned int m_Count;
 static std::unordered_map<int, int> m_Map;
};

#endif // !TRANSFORM_H

