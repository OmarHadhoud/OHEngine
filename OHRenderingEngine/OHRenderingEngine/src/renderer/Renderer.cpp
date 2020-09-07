#include "Renderer.h"


#include <GLFW/glfw3.h>

//Initialize static variables
bool Renderer::s_ClearColorBufferEnabled = true;
bool Renderer::s_ClearDepthBufferEnabled = true;
bool Renderer::s_ClearStencilBufferEnabled = true;
bool Renderer::s_AntiAliasingEnabled = false;
unsigned int Renderer::s_MultiSamples = 1;
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

void Renderer::EnableClearColorBuffer()
{
	s_ClearColorBufferEnabled = true;
}

void Renderer::DisableClearColorBuffer()
{
	s_ClearColorBufferEnabled = false;
}

void Renderer::EnableClearDepthBuffer()
{
	s_ClearDepthBufferEnabled = true;
}

void Renderer::DisableClearDepthBuffer()
{
	s_ClearDepthBufferEnabled = false;
}

void Renderer::EnableClearStencilBuffer()
{
	s_ClearStencilBufferEnabled = true;
}

void Renderer::DisableClearStencilBuffer()
{
	s_ClearStencilBufferEnabled = false;
}

void Renderer::Clear()
{
	GlCall(glClearColor(s_ClearColor.x, s_ClearColor.y , s_ClearColor.z, s_ClearColor.a));
	unsigned int clear_flag = 0;
	if (s_ClearColorBufferEnabled)
		clear_flag |= GL_COLOR_BUFFER_BIT;
	if (s_ClearDepthBufferEnabled)
		clear_flag |= GL_DEPTH_BUFFER_BIT;
	if (s_ClearStencilBufferEnabled)
		clear_flag |= GL_STENCIL_BUFFER_BIT;
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

void Renderer::DisableDepthTesting()
{
	GlCall(glDisable(GL_DEPTH_TEST));
}

void Renderer::EnableStencilTesting()
{
	GlCall(glEnable(GL_STENCIL_TEST));
}

void Renderer::DisableStencilTesting()
{
	GlCall(glDisable(GL_STENCIL_TEST));
}

void Renderer::SetDepthFunc(ComparisonFunc fn)
{
	GlCall(glDepthFunc(fn));
}

void Renderer::SetStencilOp(TestOption sfail, TestOption dpthfail, TestOption dpthpass)
{
	GlCall(glStencilOp(sfail, dpthfail, dpthpass));
}

void Renderer::SetStencilMask(unsigned int mask)
{
	GlCall(glStencilMask(mask));
}

void Renderer::SetStencilFunc(ComparisonFunc fn, int ref, unsigned int mask)
{
	GlCall(glStencilFunc(fn, ref, mask));
}

void Renderer::SetDepthMask(bool mask)
{
	GlCall(glDepthFunc(mask));
}

void Renderer::EnableAntiAliasing()
{
	GlCall(glEnable(GL_MULTISAMPLE));
	s_AntiAliasingEnabled = true;
}

void Renderer::DisableAntiAliasing()
{
	GlCall(glDisable(GL_MULTISAMPLE));
	s_AntiAliasingEnabled = false;
}

void Renderer::SetAntiAliasingSamples(unsigned int n)
{
	s_MultiSamples = n;
}
