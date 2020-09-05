#include <glad/glad.h>

#include "FrameBuffer.h"
#include "Debugger.h"

FrameBuffer::FrameBuffer(FrameBufferTarget target)
{
	GlCall(glGenFramebuffers(1, &m_ID));
	GlCall(glBindFramebuffer(target, m_ID));
	m_Target = target;
}

FrameBuffer::~FrameBuffer()
{
	GlCall(glDeleteFramebuffers(1, &m_ID));
}

void FrameBuffer::Bind() const
{
	GlCall(glBindFramebuffer(m_Target, m_ID));
}

void FrameBuffer::Unbind() const
{
	GlCall(glBindFramebuffer(m_Target, 0));
}

void FrameBuffer::AttachTexture(Texture & tex, FrameBufferAttachement attach) const
{
	GlCall(glFramebufferTexture2D(m_Target, attach, GL_TEXTURE_2D, tex.GetId(), 0));
}

void FrameBuffer::AttachRenderObject(RenderBuffer & rbo, FrameBufferAttachement attach) const
{
	GlCall(glFramebufferRenderbuffer(m_Target, attach, GL_RENDERBUFFER, rbo.GetId()));
}


