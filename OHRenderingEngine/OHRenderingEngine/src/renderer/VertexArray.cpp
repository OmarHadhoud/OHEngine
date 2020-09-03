#include "VertexArray.h"
#include "Debugger.h"


VertexArray::VertexArray()
{
	GlCall(glGenVertexArrays(1, &m_id_));
	GlCall(glBindVertexArray(m_id_));
}

static unsigned int GLSizeOf(unsigned int type)
{
	switch (type)
	{
	case GL_INT:
		return sizeof(int);
		break;
	case GL_FLOAT:
		return sizeof(float);
		break;
	case GL_UNSIGNED_INT:
		return sizeof(unsigned int);
		break;
	default:
		break;
	}
	ASSERT(0);
	return 0;
}

VertexArray::~VertexArray()
{
	GlCall(glDeleteVertexArrays(1, &m_id_));
}

void VertexArray::Bind() const
{
	GlCall(glBindVertexArray(m_id_));
}

void VertexArray::Unbind() const
{
	GlCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(VertexBuffer & vb, VertexBufferLayout & vbl)
{
	Bind();
	vb.Bind();
	unsigned int stride = vbl.GetStride();
	std::vector<VertexBufferElement> elements = vbl.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		GlCall(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, stride, (void*)offset));
		GlCall(glEnableVertexAttribArray(i));
		offset += elements[i].count*GLSizeOf(elements[i].type);
	}
}