#ifndef LIGHT_SHADOW_H
#define LIGHT_SHADOW_H

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
};

#endif // !LIGHT_SHADOW_H

