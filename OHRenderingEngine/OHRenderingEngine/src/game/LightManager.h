#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <vector>

#include "renderer/VertexBuffer.h"
#include "renderer/VertexBufferLayout.h"
#include "renderer/IndexBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/Shader.h"
#include "renderer/Renderer.h"

#include "game/DirectionaLight.h"
#include "game/PointLight.h"
#include "game/SpotLight.h" 

#define MAX_DIRECTIONAL_LIGHTS  5
#define MAX_POINT_LIGHTS  20
#define MAX_SPOTLIGHTS 20

class LightManager
{
public:
	LightManager();
	~LightManager();
	void AddLight(Light &light);
	void SetLight(Shader &shader, glm::mat4 view_matrix) const;
	void DrawLights(glm::mat4 view, glm::mat4 projection) const;
private:
	std::vector<DirectionaLight*> m_DirectionalLights;
	std::vector<PointLight*> m_PointLights;
	std::vector<SpotLight*> m_SpotLights;
	VertexArray m_VAO;
	VertexBuffer m_VBO;
	VertexBufferLayout m_VBL;
	IndexBuffer m_IBO;
	Shader m_Shader;
};

#endif // !LIGHT_MANAGER_H
