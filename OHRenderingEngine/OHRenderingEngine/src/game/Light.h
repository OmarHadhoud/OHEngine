#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 color, float ambient, float diffuse, float specular, bool enabled);
	virtual ~Light() = 0;
	glm::vec3 GetColor() const;
	float GetAmbient() const;
	float GetDiffuse() const;
	float GetSpecular() const;
	bool IsEnabled() const;
	void SetColor(glm::vec3 color);
	void SetAmbient(float ambient);
	void SetDiffuse(float diffuse);
	void SetSpecular(float specular);
	void Enable();
	void Disable();
private:
	glm::vec3 m_Color;
	float m_Ambient;
	float m_Diffuse;
	float m_Specular;
	bool m_Enabled;
};
#endif //! LIGHT_H

