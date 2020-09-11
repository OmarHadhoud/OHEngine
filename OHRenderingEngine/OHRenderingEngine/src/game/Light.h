#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 color, float ambient, float diffuse, float specular);
	virtual ~Light() = 0;
	glm::vec3 GetColor() const;
	float GetAmbient() const;
	float GetDiffuse() const;
	float GetSpecular() const;
	void SetColor(glm::vec3 color);
	void SetAmbient(float ambient);
	void SetDiffuse(float diffuse);
	void SetSpecular(float specular);

private:
	glm::vec3 m_Color;
	float m_Ambient;
	float m_Diffuse;
	float m_Specular;
};
#endif //! LIGHT_H

