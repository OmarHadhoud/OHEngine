#include "VertexArray.h"
#include "Debugger.h"


VertexArray::VertexArray():
	m_BoundedAttributes(0)
{
	GlCall(glGenVertexArrays(1, &m_ID));
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
	case GL_FLOAT_VEC2:
		return sizeof(glm::vec2);
		break;
	case GL_FLOAT_VEC3:
		return sizeof(glm::vec3);
		break;
	case GL_FLOAT_VEC4:
		return sizeof(glm::vec4);
		break;
	default:
		break;
	}
	ASSERT(0);
	return 0;
}

VertexArray::~VertexArray()
{
	GlCall(glDeleteVertexArrays(1, &m_ID));
}

VertexArray::VertexArray(VertexArray && other) noexcept
{
	//Move the ID
	m_ID = other.m_ID;

	//Clean the other buffer
	other.m_ID = 0;
}

VertexArray & VertexArray::operator=(VertexArray && other) noexcept
{
	GlCall(glDeleteVertexArrays(1, &m_ID));
	//Move the ID
	m_ID = other.m_ID;

	//Clean the other buffer
	other.m_ID = 0;

	return *this;
}

void VertexArray::Bind() const
{
	GlCall(glBindVertexArray(m_ID));
}

void VertexArray::Unbind()
{
	GlCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer & vb)
{
	const VertexBufferLayout* vbl = &vb.GetLayout();
	unsigned int stride = vbl->GetStride();
	std::vector<VertexBufferElement> elements = vbl->GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		GlCall(glVertexAttribPointer(m_BoundedAttributes, elements[i].count, elements[i].type, elements[i].normalized, stride, (void*)offset));
		GlCall(glEnableVertexAttribArray(m_BoundedAttributes));
		offset += elements[i].count*GLSizeOf(elements[i].type);
		m_BoundedAttributes++;
	}
}
