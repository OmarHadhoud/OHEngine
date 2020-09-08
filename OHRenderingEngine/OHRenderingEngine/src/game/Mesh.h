#ifndef MESH_H
#define MESH_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "renderer/IndexBuffer.h"
#include "renderer/VertexBuffer.h"
#include "renderer/VertexBufferLayout.h"
#include"renderer/VertexArray.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct TextureMaterial {
	Texture tex;
	std::string type;
};

class Mesh
{
public:
	//Mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureMaterial> textures;

	Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<TextureMaterial> &textures);
	void Draw(Shader &shader);
	~Mesh();

private:
	IndexBuffer m_IBO;
	VertexBuffer m_VBO;
	VertexBufferLayout m_VBL;
	VertexArray m_VAO;

	void setupMesh();
};

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<TextureMaterial> &textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	setupMesh();
}

void Mesh::Draw(Shader & shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		Texture::Activate(i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);		
		shader.SetInt(("material."+name+std::to_string(diffuseNr)).c_str(), i);
		textures[i].tex.Bind();
	}
	if(textures.size()>0)
		textures[0].tex.Unbind();
	m_VAO.Bind();
	Renderer::Draw(m_VAO, shader, indices.size() , 0);

	m_VAO.Unbind();
}


void Mesh::setupMesh()
{
	m_VAO.Bind();
	m_VBO = VertexBuffer(&vertices[0], sizeof(Vertex) * vertices.size(), kStaticDraw);
	m_IBO = IndexBuffer(&indices[0], sizeof(unsigned int) * indices.size(), kStaticDraw);
	//Vertex positions
	m_VBL.Push<glm::vec3>(1, false);
	//Vertex normal
	m_VBL.Push<glm::vec3>(1, false);
	//Vertex tex coordinate
	m_VBL.Push<glm::vec2>(1, false);
	//Vertex Tangent
	m_VBL.Push<glm::vec3>(1, false);
	//Vertex Bitangent
	m_VBL.Push<glm::vec3>(1, false);
	m_VBO.SetLayout(m_VBL);
	m_VAO.AddBuffer(m_VBO);
	m_VAO.Unbind();
}

Mesh::~Mesh()
{
}

#endif //!MESH_H