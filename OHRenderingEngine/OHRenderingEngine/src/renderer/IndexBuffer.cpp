#include <glad/glad.h>

#include "IndexBuffer.h"
#include "Debugger.h"

IndexBuffer::IndexBuffer()
{
	GlCall(glGenBuffers(1, &m_ID));
}

IndexBuffer::IndexBuffer(unsigned int size, BUFFER_USAGE usage = kStaticDraw)
{
	GlCall(glGenBuffers(1, &m_ID));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, usage));
	Unbind();
}

IndexBuffer::IndexBuffer(void * data, unsigned int size, BUFFER_USAGE usage = kStaticDraw)
{
	GlCall(glGenBuffers(1, &m_ID));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage));
	Unbind();
}


IndexBuffer::~IndexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_ID));
}

IndexBuffer::IndexBuffer(IndexBuffer && other) noexcept
{
	//Move the ID
	m_ID = other.m_ID;

	//Clean the other buffer
	other.m_ID = 0;
}

IndexBuffer & IndexBuffer::operator=(IndexBuffer && other) noexcept
{
	GlCall(glDeleteBuffers(1, &m_ID));
	//Move the ID
	m_ID = other.m_ID;

	//Clean the other buffer
	other.m_ID = 0;

	return *this;
}

void IndexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
}

void IndexBuffer::Unbind()
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::BufferData(void * data, unsigned int size, BUFFER_USAGE usage) const
{
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage));
}

void IndexBuffer::BufferSubData(void * data, unsigned int size, unsigned int offset) const
{
	GlCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data));
}
