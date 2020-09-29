#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/glm.hpp>

#include "game/ecs/Components/Component.h"

struct PointLight : Component
{
	glm::vec3 m_Color;
	float m_Ambient;
	float m_Diffuse;
	float m_Specular;
	float m_kConstant;
	float m_kLinear;
	float m_kQuadratic;
	static unsigned int m_Count;
 static std::unordered_map<int, int> m_Map;
};

#endif // !POINT_LIGHT_H
