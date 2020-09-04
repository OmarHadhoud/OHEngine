#include "Renderer.h"


#include <GLFW/glfw3.h>

//Initialize static variables
bool Renderer::s_ClearColorEnabled = true;
bool Renderer::s_ClearDepthEnabled = true;
glm::vec4 Renderer::s_ClearColor= glm::vec4(0.0f);


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}


void Renderer::SetClearColor(float r, float g, float b, float a)
{
	s_ClearColor = glm::vec4(r, g, b, a);
}

void Renderer::EnableClearColor()
{
	s_ClearColorEnabled = true;
}

void Renderer::DisableClearColor()
{
	s_ClearColorEnabled = false;
}

void Renderer::EnableClearDepth()
{
	s_ClearDepthEnabled = true;
}

void Renderer::DisableClearDepth()
{
	s_ClearDepthEnabled = false;
}

void Renderer::Clear()
{
	GlCall(glClearColor(s_ClearColor.x, s_ClearColor.y , s_ClearColor.z, s_ClearColor.a));
	unsigned int clear_flag = 0;
	if (s_ClearColorEnabled)
		clear_flag |= GL_COLOR_BUFFER_BIT;
	if (s_ClearDepthEnabled)
		clear_flag |= GL_DEPTH_BUFFER_BIT;

	GlCall(glClear(clear_flag));
}

void Renderer::Draw(const VertexArray & vao, const Shader & shader, unsigned int count, unsigned int offset)
{
	vao.Bind();
	shader.Use();
	GlCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)offset));
}

void Renderer::ResizeWindow(int width, int height)
{
	GlCall(glViewport(0, 0, width, height));
}

void Renderer::EnableDepthTesting()
{
	GlCall(glEnable(GL_DEPTH_TEST));
}

void Renderer::EnableAntiAliasing()
{
	GlCall(glEnable(GL_MULTISAMPLE));
}

void Renderer::SetAntiAliasingSamples(unsigned int n)
{
	glfwWindowHint(GLFW_SAMPLES, 4);
}
