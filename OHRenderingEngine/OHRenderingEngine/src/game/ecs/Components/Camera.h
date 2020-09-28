#ifndef CAMERACOMP_H
#define v

#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

//TODO: Either delete or update it and use it.
struct CameraCOMP
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

#endif // !CAMERACOMP_H

