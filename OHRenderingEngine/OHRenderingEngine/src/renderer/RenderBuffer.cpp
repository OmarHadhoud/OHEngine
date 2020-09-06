#include <glad/glad.h>

#include "RenderBuffer.h"
#include "Debugger.h"

RenderBuffer::RenderBuffer()
{
	GlCall(glGenRenderbuffers(1, &m_ID));
	GlCall(glBindRenderbuffer(GL_RENDERBUFFER, m_ID));
}

RenderBuffer::~RenderBuffer()
{
	GlCall(glDeleteRenderbuffers(1, &m_ID));
}

unsigned int RenderBuffer::GetId() const
{
	return m_ID;
}

void RenderBuffer::Bind() const
{
	GlCall(glBindRenderbuffer(GL_RENDERBUFFER, m_ID));
}

void RenderBuffer::Unbind() const
{
	GlCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void RenderBuffer::Create(float width, float height, BufferType bType) const
{
	Bind();
	if (bType == kDepth)
	{
		GlCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH, width, height));
	}
	else if (bType == kStencil)
	{
		GlCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL, width, height));
	}
	else if (bType == kDepthStencil)
	{
		GlCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	}
}

