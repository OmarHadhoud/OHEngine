#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include <vector>
#include <glm/glm.hpp>

#include "game/Collider.h"

class BoxCollider :	public Collider
{
public:
	BoxCollider();
	~BoxCollider();
	void SetMinBound(glm::vec3 min_bound);
	void SetMaxBound(glm::vec3 max_bound);
	//Returns a vector of 2 float values for t1, t2 for intersection, if they intersect. If they don't the boolean is false.
	std::vector<float> GetRayIntersection(glm::vec3 origin, glm::vec3 dir, bool &intersect) const;
private:
	glm::vec3 m_Bounds[2];
};

#endif // !BOX_COLLIDER_H
