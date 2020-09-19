#ifndef LIGHT_H
#define LIGHT_H

#include "renderer/Texture.h"

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
	float GetFrustumSize() const;
	float GetNearPlane() const;
	float GetFarPlane() const;
	glm::mat4 GetTransformationMatrix() const;
	int GetDepthMap() const;
	bool IsEnabled() const;
	void SetColor(glm::vec3 color);
	void SetAmbient(float ambient);
	void SetDiffuse(float diffuse);
	void SetSpecular(float specular);
	void SetDepthMap(int map);
	void SetFrustumSize(float size);
	void SetNearPlane(float value);
	void SetFarPlane(float value);
	virtual void UpdateTransformationMatrix() = 0;
	void Enable();
	void Disable();
private:
	glm::vec3 m_Color;
	float m_Ambient;
	float m_Diffuse;
	float m_Specular;
	bool m_Enabled;
	//Shadow mapping related
	glm::mat4 m_TransformationMatrix;
	int m_DepthMap;
	float m_FrustumSize;
	float m_NearPlane;
	float m_FarPlane;
protected:
	void SetTransformationMatrix(glm::mat4 transformation_matrix);
};
#endif //! LIGHT_H

