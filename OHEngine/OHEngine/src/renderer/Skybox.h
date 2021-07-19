#ifndef SKYBOX_H
#define SKYBOX_H

#include "renderer/VertexBuffer.h"
#include "renderer/VertexBufferLayout.h"
#include "renderer/IndexBuffer.h"
#include "renderer/VertexArray.h"
#include "renderer/CubeMapTexture.h"
#include "renderer/Shader.h"
#include "renderer/Renderer.h"

class Skybox
{
public:
	Skybox(std::vector<std::string> images);
	~Skybox();
	void Draw(glm::mat4 &view, glm::mat4 &projection) const;
private:
	VertexBuffer m_VBO;
	VertexBufferLayout m_VBL;
	IndexBuffer m_IBO;
	VertexArray m_VAO;
	CubeMapTexture m_Tex;
	Shader m_Shader;
};

#endif // !SKYBOX_H
