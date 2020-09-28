#ifndef LIGHT_SHADOW_H
#define LIGHT_SHADOW_H

#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

struct LightShadow
{
	std::vector<glm::mat4> m_TransformationMatrix;
	int m_DepthMap;
	float m_FrustumSize;
	float m_NearPlane;
	float m_FarPlane;
	bool m_Enabled;
	static unsigned int m_Count;
	static std::unordered_map<unsigned int, unsigned int> m_Map;
};

#endif // !LIGHT_SHADOW_H

