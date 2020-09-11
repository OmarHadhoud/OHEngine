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
#include "renderer/VertexArray.h"
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
	//Default constructor
	TextureMaterial() = default;
	//Default constructor with texture passed
	TextureMaterial(Texture &&tex)
		: tex(std::move(tex))
	{
		type = "";
	};
	//Copy constructor
	TextureMaterial(const TextureMaterial &other)
		: tex(other.tex), type(other.type)
	{	};
	//Move constructor
	TextureMaterial(TextureMaterial && other) noexcept
		: tex(std::move(other.tex)), type(other.type)
	{	};
	//Copy assignment operator
	TextureMaterial& operator=(const TextureMaterial &other)
	{
		tex = other.tex;
		type = other.type;
		return *this;
	};
	//Move assignment operator
	TextureMaterial& operator=(TextureMaterial &&other) noexcept
	{
		if (this != &other)
		{
			tex = std::move(other.tex);
			type = std::move(other.type);
		}
		return *this;
	};
	Texture tex;
	std::string type;
};

class Mesh
{
public:
	//Mesh data
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<unsigned int> m_TexturesIndices;

	Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<unsigned int> &textures_indices);
	//Copy constructor
	Mesh(const Mesh& other) = delete;
	//Move constructor
	Mesh(Mesh && other) noexcept;
	//Copy assignment operator
	Mesh& operator=(const Mesh& other) = delete;
	//Move assignment operator
	Mesh& operator=(Mesh&& other) noexcept;
	void Draw(Shader &shader, std::vector<TextureMaterial> &textures);
	~Mesh();
	
private:
	VertexArray m_VAO;
	IndexBuffer m_IBO;
	VertexBuffer m_VBO;
	VertexBufferLayout m_VBL;

	void setupMesh();
};

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<unsigned int> &textures_indices)
{
	this->m_Vertices = vertices;
	this->m_Indices = indices;
	this->m_TexturesIndices = textures_indices;
	setupMesh();
}

Mesh::Mesh(Mesh && other) noexcept
{
	//Steal the buffers and veretex array
	m_IBO = std::move(other.m_IBO);
	m_VBO = std::move(other.m_VBO);
	m_VAO = std::move(other.m_VAO);
	//Copy the buffer layout and vectors
	m_VBL = other.m_VBL;
	m_Vertices = other.m_Vertices;
	m_Indices = other.m_Indices;
	m_TexturesIndices = other.m_TexturesIndices;
}

Mesh & Mesh::operator=(Mesh && other) noexcept
{
	//Steal the buffers and veretex array
	m_IBO = std::move(other.m_IBO);
	m_VBO = std::move(other.m_VBO);
	m_VAO = std::move(other.m_VAO);
	//Copy the buffer layout and vectors
	m_VBL = other.m_VBL;
	m_Vertices = other.m_Vertices;
	m_Indices = other.m_Indices;
	m_TexturesIndices = other.m_TexturesIndices;

	return *this;
}

void Mesh::Draw(Shader & shader, std::vector<TextureMaterial> &textures)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int j = 0; j < m_TexturesIndices.size(); j++)
	{
		unsigned int i = m_TexturesIndices[j];
		Texture::Activate(i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else 
			continue;
		shader.SetInt(("material." + name + number.c_str()), i);
		textures[i].tex.Bind();
	}
	if (m_TexturesIndices.size() > 0)
		textures[0].tex.Activate(0);
	Renderer::Draw(m_VAO, shader, m_Indices.size(), 0);

	m_VAO.Unbind();
}


void Mesh::setupMesh()
{
	m_VAO.Bind();
	m_VBO.Bind();
	m_IBO.Bind();
	m_VBO.BufferData(&m_Vertices[0], sizeof(Vertex) * m_Vertices.size(), kStaticDraw);
	m_IBO.BufferData(&m_Indices[0], sizeof(unsigned int) * m_Indices.size(), kStaticDraw);
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
	m_VBO.Unbind();
	m_IBO.Unbind();
}

Mesh::~Mesh()
{
}

#endif //!MESH_H