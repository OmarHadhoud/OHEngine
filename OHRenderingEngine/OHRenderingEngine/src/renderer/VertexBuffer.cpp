#include <glad/glad.h>

#include "VertexBuffer.h"
#include "Debugger.h"

VertexBuffer::VertexBuffer()
{
	GlCall(glGenBuffers(1, &m_ID));
}

VertexBuffer::VertexBuffer(unsigned int size, BUFFER_USAGE usage = kStaticDraw)
{
	GlCall(glGenBuffers(1, &m_ID));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, usage));
	Unbind();
}

VertexBuffer::VertexBuffer(void * data, unsigned int size, BUFFER_USAGE usage = kStaticDraw)
{
	GlCall(glGenBuffers(1, &m_ID));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
	Unbind();
}


VertexBuffer::~VertexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_ID));
}



VertexBuffer::VertexBuffer(VertexBuffer && other) noexcept
{
	//Move the ID and layout
	m_ID = other.m_ID;
	m_Layout = other.m_Layout;

	//Clean the other buffer
	other.m_ID = 0;
}

VertexBuffer & VertexBuffer::operator=(VertexBuffer && other) noexcept
{
	GlCall(glDeleteBuffers(1, &m_ID));
	//Move the ID and layout
	m_ID = other.m_ID;
	m_Layout = other.m_Layout;

	//Clean the other buffer
	other.m_ID = 0;

	return *this;
}

void VertexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
}

void VertexBuffer::Unbind()
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::BufferData(void * data, unsigned int size, BUFFER_USAGE usage)
{
	Bind();
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
}

void VertexBuffer::BufferSubData(void * data, unsigned int size, unsigned int offset) const
{
	Bind();
	GlCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

const VertexBufferLayout& VertexBuffer::GetLayout() const
{
	return m_Layout;
}

void VertexBuffer::SetLayout(VertexBufferLayout & layout)
{
	m_Layout = layout;
}
