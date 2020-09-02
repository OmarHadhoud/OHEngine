#include <glad/glad.h>

#include "VertexBuffer.h"
#include "Debugger.h"

VertexBuffer::VertexBuffer()
{
	GlCall(glGenBuffers(1, &m_id_));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_id_));
}

VertexBuffer::VertexBuffer(unsigned int size, GLenum usage = GL_STATIC_DRAW)
{
	GlCall(glGenBuffers(1, &m_id_));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_id_));
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, usage));
}

VertexBuffer::VertexBuffer(void * data, unsigned int size, GLenum usage = GL_STATIC_DRAW)
{
	GlCall(glGenBuffers(1, &m_id_));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_id_));
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
}


VertexBuffer::~VertexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_id_));
}

void VertexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_id_));
}

void VertexBuffer::Unbind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::BufferSubData(void * data, unsigned int size, unsigned int offset) const
{
	GlCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}
