#include "Renderer.h"



Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::ClearScreen(float r, float g, float b, float a)
{
	GlCall(glClearColor(r,g,b,a));
	GlCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(VertexArray & vao, Shader & shader, unsigned int count, unsigned int offset)
{
	vao.Bind();
	shader.Use();
	GlCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)offset));
}

void Renderer::ResizeWindow(int width, int height)
{
	GlCall(glViewport(0, 0, width, height));
}
