#include "LightManager.h"

#include <glm/glm_util.hpp>

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

	4, 5, 7,
	5, 6, 7,

	8, 9, 11,
	9, 10, 11,

	12, 13, 15,
	13, 14, 15,

	16, 17, 19,
	17, 18, 19,

	20, 21, 23,
	21, 22, 23
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

void LightManager::AddLight(Light * light)
{
	DirectionaLight *dir_light = dynamic_cast<DirectionaLight*>(light);
	if (dir_light != nullptr)
	{
		m_DirectionalLights.push_back(DirectionaLight(*dir_light));
	}

	PointLight *pt_light = dynamic_cast<PointLight*>(light);
	if (pt_light != nullptr)
	{
		m_PointLights.push_back(PointLight(*pt_light));
	}

	SpotLight *spot_light = dynamic_cast<SpotLight*>(light);
	if (spot_light != nullptr)
	{
		m_SpotLights.push_back(SpotLight(*spot_light));
	}


}

void LightManager::SetLight(Shader & shader) const
{
	shader.SetInt("n_DirectionalLights", m_DirectionalLights.size());
	for (int i = 0; i < m_DirectionalLights.size(); i++)
	{
		shader.SetFloat("directionalLights[" + std::to_string(i) + "].ambient", m_DirectionalLights[i].GetAmbient());
		shader.SetFloat("directionalLights[" + std::to_string(i) + "].diffuse", m_DirectionalLights[i].GetDiffuse());
		shader.SetFloat("directionalLights[" + std::to_string(i) + "].specular", m_DirectionalLights[i].GetSpecular());
		shader.SetVec3("directionalLights[" + std::to_string(i) + "].direction", m_DirectionalLights[i].GetDirection());
		shader.SetVec3("directionalLights["+std::to_string(i)+"].color", m_DirectionalLights[i].GetColor());
	}
	
	shader.SetInt("n_PointLights", m_PointLights.size());
	for (int i = 0; i < m_PointLights.size(); i++)
	{
		shader.SetFloat("pointLights[" + std::to_string(i) + "].ambient", m_PointLights[i].GetAmbient());
		shader.SetFloat("pointLights[" + std::to_string(i) + "].diffuse", m_PointLights[i].GetDiffuse());
		shader.SetFloat("pointLights[" + std::to_string(i) + "].specular", m_PointLights[i].GetSpecular());
		shader.SetVec3("pointLights[" + std::to_string(i) + "].position", m_PointLights[i].GetPosition());
		shader.SetVec3("pointLights[" + std::to_string(i) + "].color", m_PointLights[i].GetColor());
		shader.SetFloat("pointLights[" + std::to_string(i) + "].kC", m_PointLights[i].GetConstant());
		shader.SetFloat("pointLights[" + std::to_string(i) + "].kL", m_PointLights[i].GetLinearConstant());
		shader.SetFloat("pointLights[" + std::to_string(i) + "].kQ", m_PointLights[i].GetQuadraticConstant());
	}

	shader.SetInt("n_SpotLights", m_SpotLights.size());
	for (int i = 0; i < m_SpotLights.size(); i++)
	{
		shader.SetFloat("spotLights[" + std::to_string(i) + "].ambient", m_SpotLights[i].GetAmbient());
		shader.SetFloat("spotLights[" + std::to_string(i) + "].diffuse", m_SpotLights[i].GetDiffuse());
		shader.SetFloat("spotLights[" + std::to_string(i) + "].specular", m_SpotLights[i].GetSpecular());
		shader.SetVec3("spotLights[" + std::to_string(i) + "].position", m_SpotLights[i].GetPosition());
		shader.SetVec3("spotLights[" + std::to_string(i) + "].direction", m_SpotLights[i].GetDirection());
		shader.SetVec3("spotLights[" + std::to_string(i) + "].color", m_SpotLights[i].GetColor());
		shader.SetFloat("spotLights[" + std::to_string(i) + "].kC", m_SpotLights[i].GetConstant());
		shader.SetFloat("spotLights[" + std::to_string(i) + "].kL", m_SpotLights[i].GetLinearConstant());
		shader.SetFloat("spotLights[" + std::to_string(i) + "].kQ", m_SpotLights[i].GetQuadraticConstant());
		shader.SetFloat("spotLights[" + std::to_string(i) + "].inner_cutoff", m_SpotLights[i].GetInnerCutoff());
		shader.SetFloat("spotLights[" + std::to_string(i) + "].outer_cutoff", m_SpotLights[i].GetOuterCutoff());
	}
}

void LightManager::DrawLights(glm::mat4 view, glm::mat4 projection) const
{
	m_Shader.Use();
	m_VAO.Bind();
	glm::mat4 model;
	for (int i = 0; i < m_PointLights.size(); i++)
	{
		model = glm::mat4(0.3f);
		model = glm::translate(model, m_PointLights[i].GetPosition());
		m_Shader.SetMat4("model", model);
		m_Shader.SetMat4("view", view);
		m_Shader.SetMat4("projection", projection);
		m_Shader.SetVec3("lightColor", m_PointLights[i].GetColor());
		Renderer::Draw(m_VAO, m_Shader, 6 * 6, 0);
	}

	for (int i = 0; i < m_SpotLights.size(); i++)
	{
		model = glm::mat4(0.3f);
		model = glm::translate(model, m_SpotLights[i].GetPosition());
		m_Shader.SetMat4("model", model);
		m_Shader.SetMat4("view", view);
		m_Shader.SetMat4("projection", projection);
		m_Shader.SetVec3("lightColor", m_SpotLights[i].GetColor());
		Renderer::Draw(m_VAO, m_Shader, 6 * 6, 0);
	}
	m_VAO.Unbind();
}
