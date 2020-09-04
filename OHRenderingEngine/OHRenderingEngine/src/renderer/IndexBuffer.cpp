#include <glad/glad.h>

#include "IndexBuffer.h"
#include "Debugger.h"

IndexBuffer::IndexBuffer()
{
	GlCall(glGenBuffers(1, &m_ID));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
}

IndexBuffer::IndexBuffer(unsigned int size, GLenum usage = GL_STATIC_DRAW)
{
	GlCall(glGenBuffers(1, &m_ID));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, usage));
}

IndexBuffer::IndexBuffer(void * data, unsigned int size, GLenum usage = GL_STATIC_DRAW)
{
	GlCall(glGenBuffers(1, &m_ID));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage));
}


IndexBuffer::~IndexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_ID));
}

void IndexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
}

void IndexBuffer::Unbind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::BufferSubData(void * data, unsigned int size, unsigned int offset) const
{
	GlCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
}
