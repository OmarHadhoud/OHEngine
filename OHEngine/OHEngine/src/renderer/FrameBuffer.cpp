#include <glad/glad.h>

#include "FrameBuffer.h"
#include "Debugger.h"

FrameBuffer::FrameBuffer(GLenum target)
{
	GlCall(glGenFramebuffers(1, &m_ID));
	GlCall(glBindFramebuffer(target, m_ID));
	m_Target = target;
	Unbind();
}

FrameBuffer::~FrameBuffer()
{
	GlCall(glDeleteFramebuffers(1, &m_ID));
}

unsigned int FrameBuffer::GetID() const
{
	return m_ID;
}

void FrameBuffer::Bind() const
{
	GlCall(glBindFramebuffer(m_Target, m_ID));
}

void FrameBuffer::Bind(GLenum target) const
{
	GlCall(glBindFramebuffer(target, m_ID));
}

void FrameBuffer::Unbind() const
{
	GlCall(glBindFramebuffer(m_Target, 0));
}

void FrameBuffer::AttachTexture(Texture & tex, GLenum attach, bool is_cubemap) const
{
	if (!is_cubemap)
	{
		GlCall(glFramebufferTexture2D(m_Target, attach, tex.GetType(), tex.GetId(), 0));
	}
	else
	{
		GlCall(glFramebufferTexture(m_Target, attach, tex.GetId(), 0) );
	}
}

void FrameBuffer::AttachRenderObject(RenderBuffer & rbo, GLenum attach) const
{
	GlCall(glFramebufferRenderbuffer(m_Target, attach, GL_RENDERBUFFER, rbo.GetId()));
}

bool FrameBuffer::IsComplete() const
{
	bool ret = false;
	GLenum status;
	GlCall(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (status == GL_FRAMEBUFFER_COMPLETE)
		ret = true;
	else
		std::cout << "OPENGL ISSUE: FRAME BUFFER STATUS IS: " << status << std::endl;
	return ret;
}

void FrameBuffer::DrawBuffer(GLenum buffer)
{
	GlCall(glDrawBuffer(buffer));
}

void FrameBuffer::ReadBuffer(GLenum buffer)
{
	GlCall(glReadBuffer(buffer));
}


