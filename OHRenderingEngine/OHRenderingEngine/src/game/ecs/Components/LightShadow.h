#ifndef LIGHT_SHADOW_H
#define LIGHT_SHADOW_H

#include <vector>
#include <glm/glm.hpp>

#include "game/ecs/Components/Component.h"

struct LightShadow : Component
{
	std::vector<glm::mat4> m_TransformationMatrix;
	int m_DepthMap;
	float m_FrustumSize;
	float m_NearPlane;
	float m_FarPlane;
	static unsigned int m_Count;
 static std::unordered_map<int, int> m_Map;
};

#endif // !LIGHT_SHADOW_H

