
#include <algorithm>

#include "game/BoxCollider.h"



BoxCollider::BoxCollider()
{
	m_Bounds[0] = glm::vec3(0.0f);
	m_Bounds[1] = glm::vec3(1.0f);
}


BoxCollider::~BoxCollider()
{
}

void BoxCollider::SetMinBound(glm::vec3 min_bound)
{
	m_Bounds[0] = min_bound;
}

void BoxCollider::SetMaxBound(glm::vec3 max_bound)
{
	m_Bounds[1] = max_bound;
}

std::vector<float> BoxCollider::GetRayIntersection(glm::vec3 origin, glm::vec3 dir, bool &intersect)const
{
	std::vector<float> t = { -1,-1 };
	intersect = false;

	glm::vec3 invDir = glm::vec3(1 / dir.x, 1 / dir.y, 1 / dir.z);

	float t0x = (m_Bounds[0].x - origin.x) * invDir.x;
	float t1x = (m_Bounds[1].x - origin.x) * invDir.x;
	
	if (t0x > t1x) std::swap(t0x, t1x);
	float tmin = t0x;
	float tmax = t1x;

	float t0y = (m_Bounds[0].y - origin.y) * invDir.y;
	float t1y = (m_Bounds[1].y - origin.y) * invDir.y;
	if (t0y > t1y) std::swap(t0y, t1y);
	tmin = std::max(tmin, t0y);
	tmax = std::min(tmax, t1y);


	float t0z = (m_Bounds[0].z - origin.z) * invDir.z;
	float t1z = (m_Bounds[1].z - origin.z) * invDir.z;
	if (t0z > t1z) std::swap(t0z, t1z);
	tmin = std::max(tmin, t0z);
	tmax = std::min(tmax, t1z);
	
	if (tmin > tmax)
		return t;


	t[0] = tmin;
	t[1] = tmax;
	intersect = true;

	return t;
}
