#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <unordered_map>
#include <glm/glm.hpp>

struct Transform
{
	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_RotationAxis;
	float m_RotationAngle;
	glm::vec3 m_Forward;
	glm::mat4 m_ModelMatrix;
	bool m_Enabled;
	static unsigned int m_Count;
	static std::unordered_map<unsigned int, unsigned int> m_Map;
};

#endif // !TRANSFORM_H

