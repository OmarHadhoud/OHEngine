#include <glad/glad.h>

#include "RenderBuffer.h"
#include "Debugger.h"

RenderBuffer::RenderBuffer(): m_MultiSampledEnabled(false), m_NumSamples(1)
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
	if (m_MultiSampledEnabled)
		CreateMultiSampled(width, height, bType);
	else
		CreateNonSampled(width, height, bType);
}

void RenderBuffer::EnableMultiSampled()
{
	m_MultiSampledEnabled = true;
}

void RenderBuffer::DisableMultiSampled()
{
	m_MultiSampledEnabled = false;
}

void RenderBuffer::SetMultiSamples(unsigned int samples)
{
	m_NumSamples = samples;
}

bool RenderBuffer::IsMultiSampled() const
{
	return m_MultiSampledEnabled;
}

void RenderBuffer::CreateMultiSampled(float width, float height, BufferType bType) const
{
	if (bType == kDepth)
	{
		GlCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_NumSamples, GL_DEPTH, width, height));
	}
	else if (bType == kStencil)
	{
		GlCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_NumSamples, GL_STENCIL, width, height));
	}
	else if (bType == kDepthStencil)
	{
		GlCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_NumSamples, GL_DEPTH24_STENCIL8, width, height));
	}
}

void RenderBuffer::CreateNonSampled(float width, float height, BufferType bType) const
{
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

