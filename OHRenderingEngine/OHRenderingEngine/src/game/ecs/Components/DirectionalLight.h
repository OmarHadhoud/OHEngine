#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <unordered_map>
#include <glm/glm.hpp>

struct DirectionalLight
{
	glm::vec3 m_Color;
	glm::vec3 m_Direction;
	float m_Ambient;
	float m_Diffuse;
	float m_Specular;
	bool m_Enabled;
	static unsigned int m_Count;
	static std::unordered_map<unsigned int, unsigned int> m_Map;
};

#endif // !DIRECTIONAL_LIGHT_H

