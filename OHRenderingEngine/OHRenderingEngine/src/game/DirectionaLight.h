#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H


#include "game/Light.h"

class DirectionaLight :	public Light
{
public:
	DirectionaLight();
	DirectionaLight(glm::vec3 direction, glm::vec3 color, float ambient, float diffuse, float specular, bool enabled);
	~DirectionaLight();
	glm::vec3 GetDirection() const;
	void SetDirection(glm::vec3 dir);
private:
	glm::vec3 m_Direction;
};

#endif // !DIRECTIONAL_LIGHT_H
