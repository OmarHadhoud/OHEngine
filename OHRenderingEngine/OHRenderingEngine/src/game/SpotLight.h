#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H


#include "game/PointLight.h"
class SpotLight :	public PointLight
{
public:
	SpotLight();
	SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 color, float ambient, float diffuse, float specular, float kC, float kL, float kQ, float inner_cutoff, float outer_cutoff);
	~SpotLight();
	glm::vec3 GetDirection() const;
	float GetInnerCutoff() const;
	float GetOuterCutoff() const;
	void SetDirection(glm::vec3 dir);
	void SetInnerCutoff(float val);
	void SetOuterCutoff(float val);
private:
	glm::vec3 m_Direction;
	float m_InnerCutoff;
	float m_OuterCutoff;
};

#endif // !SPOT_LIGHT_H
