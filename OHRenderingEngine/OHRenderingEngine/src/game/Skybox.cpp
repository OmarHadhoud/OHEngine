#include "Skybox.h"

//Skybox data
float skyboxVertices[] =
{
-1.0f,  1.0f, -1.0f,	//0
-1.0f, -1.0f, -1.0f,	//1
 1.0f, -1.0f, -1.0f,	//2
 1.0f, -1.0f, -1.0f,	//3
 1.0f,  1.0f, -1.0f,	//4
-1.0f,  1.0f, -1.0f,	//5

-1.0f, -1.0f,  1.0f,	//6
-1.0f, -1.0f, -1.0f,	//7
-1.0f,  1.0f, -1.0f,	//8
-1.0f,  1.0f, -1.0f,	//9
-1.0f,  1.0f,  1.0f,	//10
-1.0f, -1.0f,  1.0f,	//11

 1.0f, -1.0f, -1.0f,	//12
 1.0f, -1.0f,  1.0f,	//13
 1.0f,  1.0f,  1.0f,	//14
 1.0f,  1.0f,  1.0f,	//15
 1.0f,  1.0f, -1.0f,	//16
 1.0f, -1.0f, -1.0f,	//17

-1.0f, -1.0f,  1.0f,	//18
-1.0f,  1.0f,  1.0f,	//19
 1.0f,  1.0f,  1.0f,	//20
 1.0f,  1.0f,  1.0f,	//21
 1.0f, -1.0f,  1.0f,	//22
-1.0f, -1.0f,  1.0f,	//23

-1.0f,  1.0f, -1.0f,	//24
 1.0f,  1.0f, -1.0f,	//25
 1.0f,  1.0f,  1.0f,	//26
 1.0f,  1.0f,  1.0f,	//27
-1.0f,  1.0f,  1.0f,	//28
-1.0f,  1.0f, -1.0f,	//29

-1.0f, -1.0f, -1.0f,	//30
-1.0f, -1.0f,  1.0f,	//31
 1.0f, -1.0f, -1.0f,	//32
 1.0f, -1.0f, -1.0f,	//33
-1.0f, -1.0f,  1.0f,	//34
 1.0f, -1.0f,  1.0f		//35
};

unsigned int skyboxIndices[] =
{
	0,1,2,
	3,4,5,
	6,7,8,
	9,10,11,
	12,13,14,
	15,16,17,
	18,19,20,
	21,22,23,
	24,25,26,
	27,28,29,
	30,31,32,
	33,34,35
};


Skybox::Skybox(std::vector<std::string> images): m_Tex(images), m_Shader("res/shaders/cubemap.vert", "res/shaders/cubemap.frag")
{
	m_Shader.SetInt("skybox", 0);
	m_VAO.Bind();

	m_VBO.Bind();
	m_VBO.BufferData(skyboxVertices, 6 * 6 * 3 * sizeof(float), kStaticDraw);

	m_IBO.Bind();
	m_IBO.BufferData(skyboxIndices, 6 * 2 * 3 * sizeof(unsigned int), kStaticDraw);

	m_VBL.Push<float>(3, false);
	m_VBO.SetLayout(m_VBL);
	
	m_VAO.AddBuffer(m_VBO);

	m_Tex.Bind();
	m_Tex.SetWrap(kS, kClampToEdge);
	m_Tex.SetWrap(kR, kClampToEdge);
	m_Tex.SetWrap(kT, kClampToEdge);
	m_Tex.SetMinFilter(kLinear);
	m_Tex.SetMagFilter(kLinear);
}

Skybox::~Skybox()
{
}

void Skybox::Draw(glm::mat4 &view, glm::mat4 &projection) const
{
	Renderer::SetDepthFunc(kLEqual);
	m_Shader.Use();
	m_Shader.SetMat4("view", glm::mat4(glm::mat3(view)));
	m_Shader.SetMat4("projection", projection);
	m_VAO.Bind();
	m_Tex.Activate(0);
	m_Tex.Bind();
	Renderer::Draw(m_VAO, m_Shader, 6 * 6, 0);
	m_Tex.Unbind();
	m_VAO.Unbind();
	Renderer::SetDepthFunc(kLess);
}



