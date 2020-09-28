#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <unordered_map>
#include <glm/glm.hpp>

struct PointLight
{
	glm::vec3 m_Color;
	glm::vec3 m_Position;
	float m_Ambient;
	float m_Diffuse;
	float m_Specular;
	float m_kConstant;
	float m_kLinear;
	float m_kQuadratic;
	bool m_Enabled;
	static unsigned int m_Count;
	static std::unordered_map<unsigned int, unsigned int> m_Map;
};

#endif // !POINT_LIGHT_H

