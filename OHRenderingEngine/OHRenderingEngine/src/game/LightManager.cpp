#include "LightManager.h"

#include <glm/glm_util.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct CubeVertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 tex_coord;
};

CubeVertex vertices[] = {
	//Front face
	glm::vec3(-0.5f,0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	//0
	glm::vec3(-0.5f,-0.5f,0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	//1
	glm::vec3(0.5f,-0.5f,0.5f),		glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	//2
	glm::vec3(0.5f,0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f),	//3
	//Back face
	glm::vec3(-0.5f,0.5f,-0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	//4
	glm::vec3(-0.5f,-0.5f,-0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	//5
	glm::vec3(0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	//6
	glm::vec3(0.5f,0.5f,-0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f),	//7
	//Right face
	glm::vec3(0.5f,0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(0.0f,1.0f),	//8
	glm::vec3(0.5f,-0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,0.0f),	//9
	glm::vec3(0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	//10
	glm::vec3(0.5f,0.5f,-0.5f),		glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f,1.0f),	//11
	//Left face
	glm::vec3(-0.5f,0.5f,0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(0.0f,1.0f),	//12
	glm::vec3(-0.5f,-0.5f,0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,0.0f),	//13
	glm::vec3(-0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	//14
	glm::vec3(-0.5f,0.5f,-0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(1.0f,1.0f),	//15
	//Upper face
	glm::vec3(-0.5f,0.5f,-0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	//16
	glm::vec3(-0.5f,0.5f,0.5f),		glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	//17
	glm::vec3(0.5f,0.5f,0.5f),		glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	//18
	glm::vec3(0.5f,0.5f,-0.5f),		glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f),	//19
	//Downwards face																	
	glm::vec3(-0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 0.0f, 0.0f),	glm::vec2(0.0f,1.0f),	//20
	glm::vec3(-0.5f,-0.5f,0.5f),	glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f,0.0f),	//21
	glm::vec3(0.5f,-0.5f,0.5f),		glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f,0.0f),	//22
	glm::vec3(0.5f,-0.5f,-0.5f),	glm::vec3(1.0f, 0.0f, 1.0f),	glm::vec2(1.0f,1.0f)	//23
};
unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3,

	7, 6, 4,
	4, 6, 5,

	8, 9, 11,
	9, 10, 11,

	15, 14, 12,
	14, 13, 12,

	16, 17, 19,
	17, 18, 19,

	23, 22, 20,
	20, 22, 21
};

LightManager::LightManager()
	: m_Shader("res/shaders/lamp.vert", "res/shaders/lamp.frag")
{
	m_VAO.Bind();

	m_VBO.Bind();
	m_VBO.BufferData(vertices, 24 * sizeof(CubeVertex), kStaticDraw);

	m_IBO.Bind();
	m_IBO.BufferData(indices, 6 * 6 * sizeof(unsigned int), kStaticDraw);

	m_VBL.Push<float>(3, false);
	m_VBL.Push<float>(3, false);
	m_VBL.Push<float>(2, false);
	m_VBO.SetLayout(m_VBL);

	m_VAO.AddBuffer(m_VBO);
}


LightManager::~LightManager()
{
}

void LightManager::AddLight(Light &light)
{
	DirectionaLight *dir_light = dynamic_cast<DirectionaLight*>(&light);
	if (dir_light != nullptr)
	{
		m_DirectionalLights.push_back(dir_light);
		return;
	}
	
	//Note: Check for spotlight first as spotlight is derived from point light
	SpotLight *spot_light = dynamic_cast<SpotLight*>(&light);
	if (spot_light != nullptr)
	{
		m_SpotLights.push_back(spot_light);
		return;
	}

	PointLight *pt_light = dynamic_cast<PointLight*>(&light);
	if (pt_light != nullptr)
	{
		m_PointLights.push_back(pt_light);
		return;
	}



}

void LightManager::SetLight(Shader & shader, glm::mat4 view_matrix) const
{
	unsigned int n_Active = 0;
	for (int i = 0; i < m_DirectionalLights.size()  && n_Active < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		if (!m_DirectionalLights[i]->IsEnabled())
			continue;
		shader.SetFloat("directionalLights[" + std::to_string(n_Active) + "].ambient", m_DirectionalLights[i]->GetAmbient());
		shader.SetFloat("directionalLights[" + std::to_string(n_Active) + "].diffuse", m_DirectionalLights[i]->GetDiffuse());
		shader.SetFloat("directionalLights[" + std::to_string(n_Active) + "].specular", m_DirectionalLights[i]->GetSpecular());
		shader.SetVec3("directionalLights[" + std::to_string(n_Active) + "].direction", glm::vec3(view_matrix*glm::vec4(m_DirectionalLights[i]->GetDirection(), 0.0f))); //W = 0 as we only want to rotate the direction, not move it with the camera
		shader.SetVec3("directionalLights["+std::to_string(n_Active)+"].color", m_DirectionalLights[i]->GetColor());
		shader.SetMat4("directionalLights[" + std::to_string(n_Active) + "].TransformationMatrix", m_DirectionalLights[i]->GetTransformationMatrix());
		shader.SetInt("directionalLights[" + std::to_string(n_Active) + "].DepthMap", m_DirectionalLights[i]->GetDepthMap());
		n_Active++;
	}
	shader.SetInt("n_DirectionalLights", n_Active);
	
	n_Active = 0;
	for (int i = 0; i < m_PointLights.size() && n_Active < MAX_POINT_LIGHTS; i++)
	{
		if (!m_PointLights[i]->IsEnabled())
			continue;
		shader.SetFloat("pointLights[" + std::to_string(n_Active) + "].ambient", m_PointLights[i]->GetAmbient());
		shader.SetFloat("pointLights[" + std::to_string(n_Active) + "].diffuse", m_PointLights[i]->GetDiffuse());
		shader.SetFloat("pointLights[" + std::to_string(n_Active) + "].specular", m_PointLights[i]->GetSpecular());
		shader.SetVec3("pointLights[" + std::to_string(n_Active) + "].position", glm::vec3(view_matrix*glm::vec4(m_PointLights[i]->GetPosition(),1.0f)));
		shader.SetVec3("pointLights[" + std::to_string(n_Active) + "].color", m_PointLights[i]->GetColor());
		shader.SetFloat("pointLights[" + std::to_string(n_Active) + "].kC", m_PointLights[i]->GetConstant());
		shader.SetFloat("pointLights[" + std::to_string(n_Active) + "].kL", m_PointLights[i]->GetLinearConstant());
		shader.SetFloat("pointLights[" + std::to_string(n_Active) + "].kQ", m_PointLights[i]->GetQuadraticConstant());
		n_Active++;
	}
	shader.SetInt("n_PointLights", n_Active);

	n_Active = 0;
	for (int i = 0; i < m_SpotLights.size() && n_Active < MAX_SPOTLIGHTS; i++)
	{
		if (!m_SpotLights[i]->IsEnabled())
			continue;
		shader.SetFloat("spotLights[" + std::to_string(n_Active) + "].ambient", m_SpotLights[i]->GetAmbient());
		shader.SetFloat("spotLights[" + std::to_string(n_Active) + "].diffuse", m_SpotLights[i]->GetDiffuse());
		shader.SetFloat("spotLights[" + std::to_string(n_Active) + "].specular", m_SpotLights[i]->GetSpecular());
		shader.SetVec3("spotLights[" + std::to_string(n_Active) + "].position", glm::vec3(view_matrix*glm::vec4(m_SpotLights[i]->GetPosition(), 1.0f))); //W = 1 as we want to translate the position
		shader.SetVec3("spotLights[" + std::to_string(n_Active) + "].direction", glm::vec3(view_matrix*glm::vec4(m_SpotLights[i]->GetDirection(), 0.0f))); //W = 0 as we only want to rotate the direction, not move it with the camera
		shader.SetVec3("spotLights[" + std::to_string(n_Active) + "].color", m_SpotLights[i]->GetColor());
		shader.SetFloat("spotLights[" + std::to_string(n_Active) + "].kC", m_SpotLights[i]->GetConstant());
		shader.SetFloat("spotLights[" + std::to_string(n_Active) + "].kL", m_SpotLights[i]->GetLinearConstant());
		shader.SetFloat("spotLights[" + std::to_string(n_Active) + "].kQ", m_SpotLights[i]->GetQuadraticConstant());
		shader.SetFloat("spotLights[" + std::to_string(n_Active) + "].inner_cutoff", m_SpotLights[i]->GetInnerCutoff());
		shader.SetFloat("spotLights[" + std::to_string(n_Active) + "].outer_cutoff", m_SpotLights[i]->GetOuterCutoff());
		shader.SetMat4("spotLights[" + std::to_string(n_Active) + "].TransformationMatrix", m_SpotLights[i]->GetTransformationMatrix());
		shader.SetInt("spotLights[" + std::to_string(n_Active) + "].DepthMap", m_SpotLights[i]->GetDepthMap());
		n_Active++;
	}
	shader.SetInt("n_SpotLights", n_Active);
}

void LightManager::DrawLights(glm::mat4 view, glm::mat4 projection) const
{
	m_Shader.Use();
	m_VAO.Bind();
	glm::mat4 model;
	for (int i = 0; i < m_PointLights.size(); i++)
	{
		if (!m_PointLights[i]->IsEnabled())
			continue;
		model = glm::mat4(0.3f);
		model = glm::translate(model, m_PointLights[i]->GetPosition());
		m_Shader.SetMat4("model", model);
		m_Shader.SetMat4("view", view);
		m_Shader.SetMat4("projection", projection);
		m_Shader.SetVec3("lightColor", m_PointLights[i]->GetColor());
		Renderer::Draw(m_VAO, m_Shader, 6 * 6, 0);
	}

	for (int i = 0; i < m_SpotLights.size(); i++)
	{
		if (!m_SpotLights[i]->IsEnabled())
			continue;
		glm::vec3 pos = m_SpotLights[i]->GetPosition();
		glm::vec3 dir = m_SpotLights[i]->GetDirection();
		model = glm::mat4(1.0f);
		model = glm::translate(model, m_SpotLights[i]->GetPosition());
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		if(glm::normalize(dir) != up && glm::normalize(dir) != -up) //Simple check as lookAt doesn't work if up is same as direction vector
			model = glm::inverse(glm::lookAt(pos, pos + dir, up));
		model = glm::scale(model, glm::vec3(0.5f));
		m_Shader.SetMat4("model", model);
		m_Shader.SetMat4("view", view);
		m_Shader.SetMat4("projection", projection);
		m_Shader.SetVec3("lightColor", m_SpotLights[i]->GetColor());
		Renderer::Draw(m_VAO, m_Shader, 6 * 6, 0);
	}
	m_VAO.Unbind();
}
