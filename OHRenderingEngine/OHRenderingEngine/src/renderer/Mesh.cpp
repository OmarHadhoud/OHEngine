
#include "renderer/Mesh.h"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<unsigned int> &textures_indices)
	: m_Vertices(vertices),
	m_Indices(indices),
	m_TexturesIndices(textures_indices)
{
	SetupMesh();
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

void Mesh::PreDraw(const Shader & shader, std::vector<TextureMaterial>& textures)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	shader.Use();
	for (unsigned int j = 0; j < m_TexturesIndices.size(); j++)
	{
		unsigned int i = m_TexturesIndices[j];
		Texture::Activate(j);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else
			continue;
		shader.SetInt(("material." + name + number.c_str()), j);
		textures[i].tex.Bind();
	}
}

void Mesh::Draw(const Shader & shader, std::vector<TextureMaterial> &textures)
{
	PreDraw(shader, textures);
	Renderer::Draw(m_VAO, shader, m_Indices.size(), 0);
	m_VAO.Unbind();
	Texture::Activate(0);
}


void Mesh::SetupMesh()
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



