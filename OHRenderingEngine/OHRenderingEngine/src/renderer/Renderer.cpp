#include "Renderer.h"


#include <GLFW/glfw3.h>



Renderer::Renderer():
	m_ActiveWindow(nullptr),
	s_AntiAliasingEnabled(true),
	s_MultiSamples(1)
{
}


Renderer::~Renderer()
{
}


void Renderer::SetActiveWindow(GLFWwindow*& window)
{
	m_ActiveWindow = window;
}

void Renderer::Clear(unsigned int flag, glm::vec4 color)
{
	GlCall(glClearColor(color.x, color.y, color.z, color.a));
	GlCall(glClear(flag));
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

void Renderer::EnableBlending()
{
	GlCall(glEnable(GL_BLEND));
}

void Renderer::SetDepthFunc(ComparisonFunc fn)
{
	GlCall(glDepthFunc(fn));
}

void Renderer::SetStencilOp(TestOption sfail, TestOption dpthfail, TestOption dpthpass)
{
	GlCall(glStencilOp(sfail, dpthfail, dpthpass));
}

void Renderer::BlitNamedFrameBuffer(const FrameBuffer & fboRead, const FrameBuffer & fboDraw, unsigned int srcX0, unsigned int srcY0, unsigned int srcX1, unsigned int srcY1, unsigned int dstX0, unsigned int dstY0, unsigned int dstX1, unsigned int dstY1)
{
	fboRead.Bind(kRead);
	fboDraw.Bind(kDraw);
	GlCall(glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, GL_COLOR_BUFFER_BIT, GL_NEAREST));
}

void Renderer::SetStencilMask(unsigned int mask)
{
	GlCall(glStencilMask(mask));
}

void Renderer::SetStencilFunc(ComparisonFunc fn, int ref, unsigned int mask)
{
	GlCall(glStencilFunc(fn, ref, mask));
}

void Renderer::DisableBlending()
{
	GlCall(glDisable(GL_BLEND));
}

void Renderer::EnableCulling()
{
	GlCall(glEnable(GL_CULL_FACE));
}

void Renderer::DisableCulling()
{
	GlCall(glDisable(GL_CULL_FACE));
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
