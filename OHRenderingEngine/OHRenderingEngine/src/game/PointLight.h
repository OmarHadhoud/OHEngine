#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H


#include "game/Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(glm::vec3 pos, glm::vec3 color, float ambient, float diffuse, float specular, float kC, float kL, float kQ, bool enabled);
	~PointLight();
	glm::vec3 GetPosition() const;
	float GetConstant() const;
	float GetLinearConstant() const;
	float GetQuadraticConstant() const;
	void SetPosition(glm::vec3 pos);
	void SetConstant(float val);
	void SetLinearConstant(float val);
	void SetQuadraticConstant(float val);
private:
	glm::vec3 m_Position;
	float m_kC;
	float m_kL;
	float m_kQ;
};

#endif // !POINT_LIGHT_H
