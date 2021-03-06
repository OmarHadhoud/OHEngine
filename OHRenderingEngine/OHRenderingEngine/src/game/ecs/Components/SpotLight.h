#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <glm/glm.hpp>
#include "game/ecs/Components/Component.h"

struct SpotLight : Component
{
	glm::vec3 m_Color;
	glm::vec3 m_Direction;
	float m_Ambient;
	float m_Diffuse;
	float m_Specular;
	float m_kConstant;
	float m_GL_LINEAR;
	float m_kQuadratic;
	float m_InnerCutoff;
	float m_OuterCutoff;
	static unsigned int m_Count;
	static int m_Indices[MAX_ENTITY_COUNT];
};

#endif // !SPOT_LIGHT_H

