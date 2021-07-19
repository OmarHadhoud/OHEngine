#ifndef MESH_H
#define MESH_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "renderer/Renderer.h"
#include "renderer/IndexBuffer.h"

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
	void PreDraw(const Shader &shader, std::vector<TextureMaterial> &textures);
	void Draw(const Shader &shader, std::vector<TextureMaterial> &textures);
	~Mesh();
	
private:
	VertexArray m_VAO;
	IndexBuffer m_IBO;
	VertexBuffer m_VBO;
	VertexBufferLayout m_VBL;

	void SetupMesh();
};

#endif //!MESH_H